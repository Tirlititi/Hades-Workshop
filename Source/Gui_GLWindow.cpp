#include "Gui_GLWindow.h"

// include OpenGL
#ifdef __WXMAC__
#include <OpenGL/glu.h>
#include <OpenGL/gl.h>
#else
#include <GL/glu.h>
#include <GL/gl.h>
#endif

#define DOUBLE_MIN_VALUE	-2e38
#define DRAW_OVER_OFFSET	15

bool IsPointInTriangle2D(GLdouble x,GLdouble y,GLdouble tx1,GLdouble ty1,GLdouble tx2,GLdouble ty2,GLdouble tx3,GLdouble ty3);
vector<GLdouble> GetPointBarycenterCoef(GLdouble x,GLdouble y,GLdouble tx1,GLdouble ty1,GLdouble tx2,GLdouble ty2,GLdouble tx3,GLdouble ty3);

GLWindow::GLWindow(wxWindow* parent, int* args, int size) :
	wxGLCanvas(parent, wxID_ANY, args, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE) {
	m_context = new wxGLContext(this);
	SetSize(size, size);
	SetBackgroundStyle(wxBG_STYLE_CUSTOM);
	mouse_lefton = false;
	mouse_righton = false;
}

GLWindow::~GLWindow() {
	delete m_context;
}

void GLWindow::DisplayField(FieldTilesDataStruct* tiles, FieldWalkmeshDataStruct* walk) {
	display_type = DISPLAY_GL_TYPE_FIELD;
	field_tiles = tiles;
	field_walk = walk;
	field_camera = 0;
	field_showtiles = false;
	field_showwalk = field_walk!=NULL;
	field_walk_offset_type = 0;
	field_walk_path_highlight = -1;
	field_walk_triangle_highlight = -1;
	field_region_vertex_amount = 0;
	field_showpoint = 0;
	field_showplane = 0;
	RecomputeGeometry(false);
/*	field_tiles_quad_pos = new GLint**[field_tiles->tiles_amount];
	for (i=0;i<field_tiles->tiles_amount;i++) {
		FieldTilesTileDataStruct& t = field_tiles->tiles[i];
		FieldTilesCameraDataStruct& cam = field_tiles->camera[t.camera_id];
		field_tiles_quad_pos[i] = new GLint*[t.tile_amount];
		for (j=0;j<t.tile_amount;j++) {
			offx = t.pos_x+t.tile_pos_x[j]-cam.pos_x;
			offy = t.distance+t.tile_depth[j]-cam.distance;
			offz = -(t.pos_y+t.tile_pos_y[j]-cam.pos_y);
			field_tiles_quad_pos[i][j] = new GLint[3];
			field_tiles_quad_pos[i][j][0] = offx;
			field_tiles_quad_pos[i][j][1] = offy;
			field_tiles_quad_pos[i][j][2] = offz;
		}
	}*/
	ResetCamera();
	Prepare3DViewport(0,0,GetSize().x,GetSize().y);
/*	bool tilesel[field_tiles->tiles_amount];
	for (i=0;i<field_tiles->tiles_amount;i++)
		tilesel[i] = false;
	texture_id = new GLuint[field_tiles->tiles_amount];
	glGenTextures(field_tiles->tiles_amount,texture_id);
	for (i=0;i<field_tiles->tiles_amount;i++) {
		if (i>0)
			tilesel[i-1] = false;
		tilesel[i] = true;
		uint32_t* backimg = field_tiles->ConvertAsImageAccurate(field_camera,tilesel,true);
		for (j=0;j<field_tiles->camera[field_camera].width*field_tiles->camera[field_camera].height;j++)
			backimg[j] = (backimg[j] & 0xFF00FF00) | ((backimg[j] & 0xFF0000) >> 16) | ((backimg[j] & 0xFF) << 16);
		glBindTexture(GL_TEXTURE_2D,texture_id[i]);
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,field_tiles->camera[field_camera].width,field_tiles->camera[field_camera].height,0,GL_RGBA,GL_UNSIGNED_BYTE,backimg);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		delete[] backimg;
	}*/
	Draw();
}

void GLWindow::DisplayFieldRegion(vector<int16_t> vert) {
	if (display_type != DISPLAY_GL_TYPE_FIELD)
		return;
	DisplayFieldClear(true, false, false, false);
	unsigned int i, j;
	GLint zpos = -0x7FFF;
	field_region_vertex_amount = vert.size() / 2;
	field_region_vertex_pos = new GLint*[field_region_vertex_amount];
	for (i = 0; i < field_region_vertex_amount; i++) {
		field_region_vertex_pos[i] = new GLint[3];
		field_region_vertex_pos[i][0] = vert[2 * i];
		field_region_vertex_pos[i][1] = vert[2 * i + 1];
		for (j = 0; j < field_walk_triangle_amount; j++)
			if (IsPointInTriangle2D(vert[2 * i], vert[2 * i + 1], field_walk_triangle_pos[3 * j][0], field_walk_triangle_pos[3 * j][1], field_walk_triangle_pos[3 * j + 1][0], field_walk_triangle_pos[3 * j + 1][1], field_walk_triangle_pos[3 * j + 2][0], field_walk_triangle_pos[3 * j + 2][1])) {
				zpos = max(max(max(zpos, field_walk_triangle_pos[3 * j][2]), field_walk_triangle_pos[3 * j + 1][2]), field_walk_triangle_pos[3 * j + 2][2]) + DRAW_OVER_OFFSET;
				break;
			}
	}
	if (zpos == -0x7FFF) {
		if (field_walk != NULL)
			zpos = -field_walk->offset_z;
		else
			zpos = 0.0f;
	}
	for (i = 0; i < field_region_vertex_amount; i++)
		field_region_vertex_pos[i][2] = zpos;
	Draw();
}

void GLWindow::DisplayFieldPolygon(vector<int16_t> vert) {
	if (display_type != DISPLAY_GL_TYPE_FIELD)
		return;
	DisplayFieldClear(true, false, false, false);
	unsigned int i;
	field_region_vertex_amount = vert.size() / 3;
	field_region_vertex_pos = new GLint*[field_region_vertex_amount];
	for (i = 0; i < field_region_vertex_amount; i++) {
		field_region_vertex_pos[i] = new GLint[3];
		field_region_vertex_pos[i][0] = vert[3 * i];
		field_region_vertex_pos[i][1] = vert[3 * i + 1];
		field_region_vertex_pos[i][2] = -vert[3 * i + 2];
	}
	Draw();
}

void GLWindow::DisplayFieldPoint3D(int16_t x, int16_t y, int16_t z) {
	if (display_type!=DISPLAY_GL_TYPE_FIELD)
		return;
	field_showpoint = 1;
	field_pointx[0] = x;
	field_pointy[0] = y;
	field_pointz[0] = -z;
	Draw();
}

void GLWindow::DisplayFieldPoint2D(int16_t x, int16_t y) {
	if (display_type != DISPLAY_GL_TYPE_FIELD)
		return;
	vector<GLdouble> barpos;
	GLdouble minbarpos, maxbarpos = DOUBLE_MIN_VALUE;
	unsigned int i;
	field_showpoint = 0;
	for (i = 0; i < field_walk_triangle_amount; i++) {
		barpos = GetPointBarycenterCoef(x, y, field_walk_triangle_pos[3 * i][0], field_walk_triangle_pos[3 * i][1], field_walk_triangle_pos[3 * i + 1][0], field_walk_triangle_pos[3 * i + 1][1], field_walk_triangle_pos[3 * i + 2][0], field_walk_triangle_pos[3 * i + 2][1]);
		if (barpos.size() == 3) {
			minbarpos = min(min(barpos[0], barpos[1]), barpos[2]);
			if (minbarpos >= 0 && field_showpoint < GL_WINDOW_FIELD_MAX_POINTS) {
				field_pointz[field_showpoint++] = barpos[0] * field_walk_triangle_pos[3 * i][2] + barpos[1] * field_walk_triangle_pos[3 * i + 1][2] + barpos[2] * field_walk_triangle_pos[3 * i + 2][2] + DRAW_OVER_OFFSET;
			} else if (minbarpos > maxbarpos && field_showpoint == 0) {
				if (max(max(barpos[0], barpos[1]), barpos[2]) == barpos[0])
					field_pointz[0] = field_walk_triangle_pos[3 * i][2] + DRAW_OVER_OFFSET;
				else if (max(max(barpos[0], barpos[1]), barpos[2]) == barpos[1])
					field_pointz[0] = field_walk_triangle_pos[3 * i + 1][2] + DRAW_OVER_OFFSET;
				else
					field_pointz[0] = field_walk_triangle_pos[3 * i + 2][2] + DRAW_OVER_OFFSET;
				maxbarpos = minbarpos;
			}
		}
	}
	if (field_showpoint == 0) {
		field_showpoint = 1;
		if (maxbarpos == DOUBLE_MIN_VALUE)
			field_pointz[0] = 0.0f;
	}
	for (i = 0; i < field_showpoint; i++) {
		field_pointx[i] = x;
		field_pointy[i] = y;
	}
	Draw();
}

void GLWindow::DisplayFieldPlane(int planetype, int16_t coord) {
	if (display_type!=DISPLAY_GL_TYPE_FIELD)
		return;
	bool mustdraw = planetype!=field_showplane;
	field_showplane = planetype;
	if (field_showplane==3)
		field_plane_coord = -coord;
	else
		field_plane_coord = coord;
	if (mustdraw)
		Draw();
}

void GLWindow::DisplayFieldTrianglePath(int triangleid) {
	if (display_type!=DISPLAY_GL_TYPE_FIELD)
		return;
	field_walk_triangle_highlight = triangleid;
	Draw();
}

void GLWindow::DisplayFieldPath(int pathid) {
	if (display_type!=DISPLAY_GL_TYPE_FIELD)
		return;
	field_walk_path_highlight = pathid;
	Draw();
}

void GLWindow::DisplayFieldClear(bool clearpolygon, bool clearpoint, bool clearplane, bool clearhighlight) {
	if (display_type!=DISPLAY_GL_TYPE_FIELD)
		return;
	bool mustdraw = false;
	if (clearpolygon && field_region_vertex_amount>0) {
		for (unsigned int i=0;i<field_region_vertex_amount;i++)
			delete[] field_region_vertex_pos[i];
		delete[] field_region_vertex_pos;
		field_region_vertex_amount = 0;
		mustdraw = true;
	}
	if (clearpoint && field_showpoint>0) {
		field_showpoint = 0;
		mustdraw = true;
	}
	if (clearplane && field_showplane>0) {
		field_showplane = 0;
		mustdraw = true;
	}
	if (clearhighlight) {
		if (field_walk_triangle_highlight >= 0) {
			field_walk_triangle_highlight = -1;
			mustdraw = true;
		}
		if (field_walk_path_highlight >= 0) {
			field_walk_path_highlight = -1;
			mustdraw = true;
		}
	}
	if (mustdraw)
		Draw();
}

void GLWindow::RecomputeGeometry(bool redraw) {
	GLint offx, offy, offz;
	unsigned int i, j;
	field_walk_triangle_amount = 0;
	if (field_walk != NULL)
		field_walk_triangle_amount = field_walk->triangle_amount;
	field_walk_triangle_pos.clear();
	for (i = 0; i < field_walk_triangle_amount; i++) {
		if (field_walk_offset_type == 0) {
			offx = field_walk->offset_orgx + field_walk->walkpath_orgx[field_walk->triangle_walkpath[i]];
			offy = field_walk->offset_orgy + field_walk->walkpath_orgy[field_walk->triangle_walkpath[i]];
			offz = -field_walk->offset_orgz - field_walk->walkpath_orgz[field_walk->triangle_walkpath[i]];
		} else if (field_walk_offset_type == 1) {
			offx = field_walk->offset_x + field_walk->walkpath_offsetx[field_walk->triangle_walkpath[i]];
			offy = field_walk->offset_y + field_walk->walkpath_offsety[field_walk->triangle_walkpath[i]];
			offz = -field_walk->offset_z - field_walk->walkpath_offsetz[field_walk->triangle_walkpath[i]];
		} else if (field_walk_offset_type == 2) {
			offx = field_walk->offset_minx + field_walk->walkpath_minx[field_walk->triangle_walkpath[i]];
			offy = field_walk->offset_miny + field_walk->walkpath_miny[field_walk->triangle_walkpath[i]];
			offz = -field_walk->offset_minz - field_walk->walkpath_minz[field_walk->triangle_walkpath[i]];
		} else {
			offx = field_walk->offset_maxx + field_walk->walkpath_maxx[field_walk->triangle_walkpath[i]];
			offy = field_walk->offset_maxy + field_walk->walkpath_maxy[field_walk->triangle_walkpath[i]];
			offz = -field_walk->offset_maxz - field_walk->walkpath_maxz[field_walk->triangle_walkpath[i]];
		}
		for (j = 0; j < 3; j++)
			field_walk_triangle_pos.push_back({ offx + field_walk->vertex_x[field_walk->triangle_vertex[i].index[j]], offy + field_walk->vertex_y[field_walk->triangle_vertex[i].index[j]], offz - field_walk->vertex_z[field_walk->triangle_vertex[i].index[j]] });
	}
	if (redraw)
		Draw();
}

int GLWindow::GetRayTriangle(GLdouble originx, GLdouble originy, GLdouble originz, GLdouble destx, GLdouble desty, GLdouble destz) {
	GLdouble dx = destx - originx;
	GLdouble dy = desty - originy;
	GLdouble dz = destz - originz;
	GLdouble eq1cc, eq1cx, eq1cy, eq1cz;
	GLdouble eq2cc, eq2cx, eq2cy, eq2cz;
	GLdouble vox, voy, voz;
	GLdouble vsx, vsy, vsz;
	GLdouble vtx, vty, vtz;
	GLdouble eqq1cc, eqq1cs, eqq1ct;
	GLdouble eqq2cc, eqq2cs, eqq2ct;
	GLdouble soldenom, sols, solt;
	GLdouble soldx, soldy, soldz;
	GLdouble soldist, closesoldist = DBL_MAX;
	int result = -1;
	if (abs(dx) > 0.0001) {
		eq1cc = -(destx * originy + destx * originz - desty * originx - destz * originx) / dx;
		eq1cx = -(dy + dz) / dx;
		eq1cy = 1.0;
		eq1cz = 1.0;
	} else {
		eq1cc = -destx;
		eq1cx = 1.0;
		eq1cy = 0.0;
		eq1cz = 0.0;
	}
	if (abs(dy) > 0.0001) {
		eq2cc = -(desty * originx + desty * originz - destx * originy - destz * originy) / dy;
		eq2cy = -(dx + dz) / dy;
		eq2cx = 1.0;
		eq2cz = 1.0;
	} else {
		eq2cc = -desty;
		eq2cy = 1.0;
		eq2cx = 0.0;
		eq2cz = 0.0;
	}
	for (unsigned int i = 0; i < field_walk_triangle_amount; i++) {
		vox = field_walk_triangle_pos[3 * i][0];
		voy = field_walk_triangle_pos[3 * i][1];
		voz = field_walk_triangle_pos[3 * i][2];
		vsx = field_walk_triangle_pos[3 * i + 1][0] - vox;
		vsy = field_walk_triangle_pos[3 * i + 1][1] - voy;
		vsz = field_walk_triangle_pos[3 * i + 1][2] - voz;
		vtx = field_walk_triangle_pos[3 * i + 2][0] - vox;
		vty = field_walk_triangle_pos[3 * i + 2][1] - voy;
		vtz = field_walk_triangle_pos[3 * i + 2][2] - voz;
		eqq1cc = eq1cc + eq1cx * vox + eq1cy * voy + eq1cz * voz;
		eqq1cs = eq1cx * vsx + eq1cy * vsy + eq1cz * vsz;
		eqq1ct = eq1cx * vtx + eq1cy * vty + eq1cz * vtz;
		eqq2cc = eq2cc + eq2cx * vox + eq2cy * voy + eq2cz * voz;
		eqq2cs = eq2cx * vsx + eq2cy * vsy + eq2cz * vsz;
		eqq2ct = eq2cx * vtx + eq2cy * vty + eq2cz * vtz;
		soldenom = eqq1cs * eqq2ct - eqq1ct * eqq2cs;
		if (abs(soldenom) > 0.0001) {
			sols = (eqq1ct * eqq2cc - eqq1cc * eqq2ct) / soldenom;
			solt = (eqq2cs * eqq1cc - eqq2cc * eqq1cs) / soldenom;
			if (sols >= 0.0 && solt >= 0.0 && sols + solt <= 1.0) {
				soldx = vox + sols * vsx + solt * vtx - originx;
				soldy = voy + sols * vsy + solt * vty - originy;
				soldz = voz + sols * vsz + solt * vtz - originz;
				soldist = soldx * dx + soldy * dy + soldz * dz;
				if (soldist >= 0.0 && soldist < closesoldist) {
					closesoldist = soldist;
					result = i;
				}
			}
		}
	}
	return result;
}

int GLWindow::GetMouseTriangle(wxMouseEvent& event) {
	wxPoint mousepos = event.GetPosition();
	GLdouble mousex = (2.0 * mousepos.x) / (double)GetSize().x - 1.0;
	GLdouble mousey = 1.0 - (2.0 * mousepos.y) / (double)GetSize().y;
	GLdouble dirx = camera_pos_x - camera_eye_x;
	GLdouble diry = camera_pos_y - camera_eye_y;
	GLdouble dirz = camera_pos_z - camera_eye_z;
	GLdouble dirnorm = sqrt(dirx * dirx + diry * diry + dirz * dirz);
	GLdouble screenhalfwidth = dirnorm * tan(65.0 * M_PI / 360.0);
	GLdouble screenxdx = sin(camera_angle_xy) * screenhalfwidth;
	GLdouble screenxdy = -cos(camera_angle_xy) * screenhalfwidth;
	GLdouble screenxdz = 0.0;
	GLdouble screenydx = sin(camera_angle_z) * cos(camera_angle_xy) * screenhalfwidth;
	GLdouble screenydy = sin(camera_angle_z) * sin(camera_angle_xy) * screenhalfwidth;
	GLdouble screenydz = cos(camera_angle_z) * screenhalfwidth;
	GLdouble destx = camera_pos_x + mousex * screenxdx + mousey * screenydx;
	GLdouble desty = camera_pos_y + mousex * screenxdy + mousey * screenydy;
	GLdouble destz = camera_pos_z + mousex * screenxdz + mousey * screenydz;
	return GetRayTriangle(camera_eye_x, camera_eye_y, camera_eye_z, destx, desty, destz);
}

void GLWindow::OnResize(wxSizeEvent& evt) {
//	wxGLCanvas::OnSize(evt);
	Refresh();
}

void GLWindow::Prepare3DViewport(int topleft_x, int topleft_y, int bottomrigth_x, int bottomrigth_y) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glViewport(topleft_x, topleft_y, bottomrigth_x-topleft_x, bottomrigth_y-topleft_y);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(65.0,1.0,1.0,100000.0L);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void GLWindow::Prepare2DViewport(int topleft_x, int topleft_y, int bottomrigth_x, int bottomrigth_y) {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black Background
	glEnable(GL_TEXTURE_2D);   // textures
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glViewport(topleft_x, topleft_y, bottomrigth_x-topleft_x, bottomrigth_y-topleft_y);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(topleft_x, bottomrigth_x, bottomrigth_y, topleft_y);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void GLWindow::ResetCamera() {
	if (field_tiles != NULL) {
		FieldTilesCameraDataStruct& cam = field_tiles->camera[field_camera];
/*		camera_pos_x = cam.offset_centerx+cam.offset_width/2.0-160.0;
		camera_pos_y = -(cam.offset_centery+cam.offset_height/2.0)+112.0;
		camera_pos_z = 0.0;
		camera_eye_x = camera_pos_x + cam.matrix[0]*cam.offset_x/4096.0 - cam.matrix[1]*cam.offset_z/4096.0 + cam.matrix[2]*cam.offset_y/4096.0;
		camera_eye_y = camera_pos_y + cam.matrix[3]*cam.offset_x/4096.0 - cam.matrix[4]*cam.offset_z/4096.0 + cam.matrix[5]*cam.offset_y/4096.0;
		camera_eye_z = camera_eye_z + cam.matrix[6]*cam.offset_x/4096.0 - cam.matrix[7]*cam.offset_z/4096.0 + cam.matrix[8]*cam.offset_y/4096.0;*/
/*		camera_eye_x = -cam.matrix[0] * cam.offset_x / 4096.0 + cam.matrix[3] * cam.offset_z / 4096.0 - cam.matrix[6] * cam.offset_y / 4096.0;
		camera_eye_y = -cam.matrix[1] * cam.offset_x / 4096.0 + cam.matrix[4] * cam.offset_z / 4096.0 - cam.matrix[7] * cam.offset_y / 4096.0;
		camera_eye_z = -cam.matrix[2] * cam.offset_x / 4096.0 + cam.matrix[5] * cam.offset_z / 4096.0 - cam.matrix[8] * cam.offset_y / 4096.0;
		camera_pos_x = camera_eye_x + cam.matrix[6] * cam.distance / 4096.0;
		camera_pos_y = camera_eye_y + cam.matrix[7] * cam.distance / 4096.0;
		camera_pos_z = camera_eye_z + cam.matrix[8] * cam.distance / 4096.0;
		camera_eye_z *= -1.0;
		camera_pos_z *= -1.0;*/
		camera_pos_x = (cam.min_x + cam.max_x) / 2;
		camera_pos_y = -(cam.min_y + cam.max_y) / 2;
		camera_pos_z = 0.0;
		camera_eye_x = cam.matrix[2];
		camera_eye_y = -cam.matrix[4];
		camera_eye_z = -cam.matrix[5];
/*fstream fout("aaaa.txt",ios::app|ios::out); for (unsigned int i=0;i<9;i++) fout << (int)cam.matrix[i] << " ";
fout << "; T " << (int)cam.offset_x << " " << (int)cam.offset_y << " " << (int)cam.offset_z;
fout << " ; V " << (float)camera_pos_x << " " << (float)camera_pos_y << " " << (float)camera_pos_z << endl;
fout << "CX " << (int)cam.offset_centerx << " CY "  << (int)cam.offset_centery;
fout << " W " << (int)cam.offset_width << " H "  << (int)cam.offset_height;
fout << " D " << (unsigned int)cam.distance << " DD "  << (int)cam.depth << endl;
fout.close();*/
		camera_angle_xy = atan2(camera_pos_y - camera_eye_y, camera_pos_x - camera_eye_x);
		camera_angle_z = atan2(camera_eye_z - camera_pos_z, sqrt((camera_pos_x - camera_eye_x) * (camera_pos_x - camera_eye_x) + (camera_pos_y - camera_eye_y) * (camera_pos_y - camera_eye_y)));
		camera_fovy = atan2(cam.offset_height / 2, cam.distance); // 65.0;
		camera_aspect = cam.offset_width * 1.0f / cam.offset_height; // 1.0;
		camera_znear = 1.0;
		camera_zfar = 100000.0;
	} else {
		camera_pos_x = 0.0;
		camera_pos_y = 0.0;
		camera_pos_z = 0.0;
		camera_eye_x = 0.0;
		camera_eye_y = 0.0;
		camera_eye_z = 1000.0;
		camera_angle_xy = (GLfloat)(M_PI / 2);
		camera_angle_z = (GLfloat)(M_PI / 2);
		camera_fovy = 65.0;
		camera_aspect = 1.0;
		camera_znear = 1.0;
		camera_zfar = 100000.0;
	}
}

void GLWindow::Draw() {
	unsigned int i;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluPerspective(65.0,1.0,1.0,100000.0L);
	gluLookAt(camera_eye_x,camera_eye_y,camera_eye_z,camera_pos_x,camera_pos_y,camera_pos_z,cos(camera_angle_xy)*sin(camera_angle_z),sin(camera_angle_xy)*sin(camera_angle_z),cos(camera_angle_z));
	if (display_type==DISPLAY_GL_TYPE_FIELD) {
		if (field_showtiles) {
			uint32_t* tileimg = field_tiles->ConvertAsImageAccurate(field_camera);
			for (i=0;i<field_tiles->camera[field_camera].width*field_tiles->camera[field_camera].height;i++)
				tileimg[i] = (tileimg[i] & 0xFF00FF00) | ((tileimg[i] & 0xFF0000) >> 16) | ((tileimg[i] & 0xFF) << 16);
			glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			glLoadIdentity();
			glOrtho(-160, 160, -160, 160, -1.0, 1.0);
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glLoadIdentity();
			glColor3f(1,1,1);
			glEnable(GL_TEXTURE_2D);
			GLuint textureid;
			glGenTextures(1,&textureid);
			glBindTexture(GL_TEXTURE_2D,textureid);
			glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,field_tiles->camera[field_camera].width,field_tiles->camera[field_camera].height,0,GL_RGBA,GL_UNSIGNED_BYTE,tileimg);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			GLint sx = field_tiles->camera[field_camera].width/2;
			GLint sy = field_tiles->camera[field_camera].height/2;
			glBegin(GL_QUADS);
			glTexCoord2f(0, 1); glVertex3f(-sx, -sy, 0);
			glTexCoord2f(0, 0); glVertex3f(-sx, sy, 0);
			glTexCoord2f(1, 0); glVertex3f(sx, sy, 0);
			glTexCoord2f(1, 1); glVertex3f(sx, -sy, 0);
			glEnd();
			glDisable(GL_TEXTURE_2D);
			glPopMatrix();
			glMatrixMode(GL_PROJECTION);
			glPopMatrix();
			glMatrixMode(GL_MODELVIEW);
			glClear(GL_DEPTH_BUFFER_BIT);
			delete[] tileimg;
/*			glEnable( GL_TEXTURE_2D );
			for (i=0;i<field_tiles->tiles_amount;i++) {
				FieldTilesTileDataStruct& t = field_tiles->tiles[i];
				if (t.camera_id!=field_camera || (!t.is_static && !t.is_first_of_anim))
					continue;
				glBindTexture(GL_TEXTURE_2D,texture_id[i]);
				glPushMatrix();
				glLoadIdentity();
				for (j=0;j<t.tile_amount;j++) {
					glBegin(GL_QUADS);
					glTexCoord2f(0, 1); glVertex3iv(field_tiles_quad_pos[i][j]);
					glTexCoord2f(0, 0); glVertex3i(field_tiles_quad_pos[i][j][0]+FIELD_TILE_BASE_SIZE,field_tiles_quad_pos[i][j][1],field_tiles_quad_pos[i][j][2]);
					glTexCoord2f(1, 0); glVertex3i(field_tiles_quad_pos[i][j][0]+FIELD_TILE_BASE_SIZE,field_tiles_quad_pos[i][j][1],field_tiles_quad_pos[i][j][2]+FIELD_TILE_BASE_SIZE);
					glTexCoord2f(1, 1); glVertex3i(field_tiles_quad_pos[i][j][0],field_tiles_quad_pos[i][j][1],field_tiles_quad_pos[i][j][2]+FIELD_TILE_BASE_SIZE);
					glEnd();
				}
				glPopMatrix();
			}
			glDisable(GL_TEXTURE_2D);*/
		}
		if (field_showwalk) {
			for (i = 0; i < field_walk_triangle_amount; i++) {
				if (i == field_walk_triangle_highlight)
					glColor4f(1, 0.9f, 0, 1);
				else if (field_walk->triangle_walkpath[i] == field_walk_path_highlight)
					glColor4f(1, 0.6f, 0, 1);
				else if ((field_walk->triangle_flag[i] & WALKMESH_TRIFLAG_ACTIVE) == 0)
					glColor4f(0.4f, 0.4f, 0.4f, 1);
				else if ((field_walk->triangle_flag[i] & WALKMESH_TRIFLAG_PREVENT_NPC) != 0)
					glColor4f(0.85f, 0.15f, 0.15f, 1);
				else if ((field_walk->triangle_flag[i] & WALKMESH_TRIFLAG_PREVENT_PC) != 0)
					glColor4f(0.7f, 0.3f, 0.3f, 1);
				else
					glColor4f(1, 0, 0, 1);
				glBegin(GL_TRIANGLES);
				glVertex3iv(field_walk_triangle_pos[3 * i].data());
				glVertex3iv(field_walk_triangle_pos[3 * i + 1].data());
				glVertex3iv(field_walk_triangle_pos[3 * i + 2].data());
				glEnd();
				glColor4f(0, 1, 0, 1);
				glBegin(GL_LINE_LOOP);
				glVertex3iv(field_walk_triangle_pos[3 * i].data());
				glVertex3iv(field_walk_triangle_pos[3 * i + 1].data());
				glVertex3iv(field_walk_triangle_pos[3 * i + 2].data());
				glEnd();
			}
		}
		if (field_region_vertex_amount>0) {
			glColor4f(0, 0, 1, 0.8f);
			glBegin(GL_POLYGON);
			for (i=0;i<field_region_vertex_amount;i++)
				glVertex3iv(field_region_vertex_pos[i]);
			glEnd();
		}
		if (field_showpoint>0) {
			glColor4f(0, 0, 1, 1);
			glPointSize(15);
			glBegin(GL_POINTS);
			for (i=0;i<field_showpoint;i++)
				glVertex3i(field_pointx[i],field_pointy[i],field_pointz[i]);
			glEnd();
			glPointSize(1);
		}
		if (field_showplane>0) {
			glColor4f(0, 0.3f, 1, 0.5f);
			glBegin(GL_QUADS);
			if (field_showplane==1) {
				glVertex3i(field_plane_coord,-10000,-10000);
				glVertex3i(field_plane_coord,10000,-10000);
				glVertex3i(field_plane_coord,10000,10000);
				glVertex3i(field_plane_coord,-10000,10000);
			} else if (field_showplane==2) {
				glVertex3i(-10000,field_plane_coord,-10000);
				glVertex3i(10000,field_plane_coord,-10000);
				glVertex3i(10000,field_plane_coord,10000);
				glVertex3i(-10000,field_plane_coord,10000);
			} else if (field_showplane==3) {
				glVertex3i(-10000,-10000,field_plane_coord);
				glVertex3i(10000,-10000,field_plane_coord);
				glVertex3i(10000,10000,field_plane_coord);
				glVertex3i(-10000,10000,field_plane_coord);
			}
			glEnd();
		}
	}
	glFlush();
	SwapBuffers();
}

void GLWindow::OnPaint( wxPaintEvent& evt ) {
	if(!IsShown()) return;
	wxGLCanvas::SetCurrent(*m_context);
	wxPaintDC(this);
	Draw();
}

#define MOUSE_TRANSLATE_FACTOR	0.005f
#define MOUSE_ROTATE_FACTOR		0.03f
void GLWindow::OnMouseMove(wxMouseEvent& event) {
	int x,y;
	event.GetPosition(&x,&y);
	if (mouse_lefton) {
		camera_angle_xy -= MOUSE_ROTATE_FACTOR*(x-mouse_x);
		camera_angle_z += MOUSE_ROTATE_FACTOR*(y-mouse_y);
		camera_angle_z = min(max((double)camera_angle_z,-M_PI/2),M_PI/2);
		GLdouble dp = sqrt((camera_eye_x-camera_pos_x)*(camera_eye_x-camera_pos_x)+(camera_eye_y-camera_pos_y)*(camera_eye_y-camera_pos_y)+(camera_eye_z-camera_pos_z)*(camera_eye_z-camera_pos_z));
		GLdouble cxy = cos(camera_angle_xy+M_PI);
		GLdouble sxy = sin(camera_angle_xy+M_PI);
		GLdouble cz = cos(camera_angle_z);
		GLdouble sz = sin(camera_angle_z);
		camera_eye_x = camera_pos_x+dp*cxy*cz;
		camera_eye_y = camera_pos_y+dp*sxy*cz;
		camera_eye_z = camera_pos_z+dp*sz;
	}
	if (mouse_righton) {
		GLdouble tn = MOUSE_TRANSLATE_FACTOR*sqrt((camera_eye_x-camera_pos_x)*(camera_eye_x-camera_pos_x)+(camera_eye_y-camera_pos_y)*(camera_eye_y-camera_pos_y)+(camera_eye_z-camera_pos_z)*(camera_eye_z-camera_pos_z));
		GLdouble tx = tn*((y-mouse_y)*cos(camera_angle_xy)-(x-mouse_x)*sin(camera_angle_xy));
		GLdouble ty = tn*((x-mouse_x)*cos(camera_angle_xy)+(y-mouse_y)*sin(camera_angle_xy));
		camera_eye_x += tx;
		camera_eye_y += ty;
		camera_pos_x += tx;
		camera_pos_y += ty;
	}
	mouse_x = x;
	mouse_y = y;
	if (mouse_lefton || mouse_righton)
		Draw();
}
void GLWindow::OnMouseWheel(wxMouseEvent& event) {
	if (event.GetWheelRotation()>0) {
		camera_eye_x = camera_eye_x*0.9+camera_pos_x*0.1;
		camera_eye_y = camera_eye_y*0.9+camera_pos_y*0.1;
		camera_eye_z = camera_eye_z*0.9+camera_pos_z*0.1;
	} else {
		camera_eye_x = camera_eye_x*1.1-camera_pos_x*0.1;
		camera_eye_y = camera_eye_y*1.1-camera_pos_y*0.1;
		camera_eye_z = camera_eye_z*1.1-camera_pos_z*0.1;
	}
	Draw();
}
void GLWindow::OnLeftClick(wxMouseEvent& event) {mouse_lefton = true; SetFocus();}
void GLWindow::OnLeftRelease(wxMouseEvent& event) {mouse_lefton = false;}
void GLWindow::OnRightClick(wxMouseEvent& event) {mouse_righton = true; SetFocus();}
void GLWindow::OnRightRelease(wxMouseEvent& event) {mouse_righton = false;}
void GLWindow::OnMouseLeaveWindow(wxMouseEvent& event) {mouse_lefton = false; mouse_righton = false;}
void GLWindow::OnKeyPress(wxKeyEvent& event) {
	int key = event.GetKeyCode();
	if (key==WXK_SPACE) {
		ResetCamera();
		Draw();
		event.Skip();
	}
}

BEGIN_EVENT_TABLE(GLWindow, wxGLCanvas)
EVT_PAINT(GLWindow::OnPaint)
EVT_SIZE(GLWindow::OnResize)
EVT_MOTION(GLWindow::OnMouseMove)
EVT_MOUSEWHEEL(GLWindow::OnMouseWheel)
EVT_LEFT_DOWN(GLWindow::OnLeftClick)
EVT_LEFT_UP(GLWindow::OnLeftRelease)
EVT_RIGHT_DOWN(GLWindow::OnRightClick)
EVT_RIGHT_UP(GLWindow::OnRightRelease)
EVT_LEAVE_WINDOW(GLWindow::OnMouseLeaveWindow)
EVT_KEY_DOWN(GLWindow::OnKeyPress)
END_EVENT_TABLE()

bool IsPointInTriangle2D(GLdouble x, GLdouble y, GLdouble tx1, GLdouble ty1, GLdouble tx2, GLdouble ty2, GLdouble tx3, GLdouble ty3) {
	GLdouble det = (tx2 - tx1) * (ty3 - ty1) - (tx3 - tx1) * (ty2 - ty1);
	GLdouble s = (ty3 - ty1) * (x - tx1) + (tx1 - tx3) * (y - ty1);
	GLdouble t = (ty1 - ty2) * (x - tx1) + (tx2 - tx1) * (y - ty1);
	if (det < 0)
		return s <= 0 && s >= det && t <= 0 && t >= det && s + t >= det;
	return s >= 0 && s <= det && t >= 0 && t <= det && s + t <= det;
}

vector<GLdouble> GetPointBarycenterCoef(GLdouble x, GLdouble y, GLdouble tx1, GLdouble ty1, GLdouble tx2, GLdouble ty2, GLdouble tx3, GLdouble ty3) {
	GLdouble det = (tx2 - tx1) * (ty3 - ty1) - (tx3 - tx1) * (ty2 - ty1);
	if (det == 0.0) // flat triangle
		return vector<GLdouble>();
	vector<GLdouble> res(3);
	res[1] = ((ty3 - ty1) * (x - tx1) + (tx1 - tx3) * (y - ty1)) / det;
	res[2] = ((ty1 - ty2) * (x - tx1) + (tx2 - tx1) * (y - ty1)) / det;
	res[0] = 1 - res[1] - res[2];
	return res;
}


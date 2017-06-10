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
GLdouble* GetPointBarycenterCoef(GLdouble x,GLdouble y,GLdouble tx1,GLdouble ty1,GLdouble tx2,GLdouble ty2,GLdouble tx3,GLdouble ty3);

GLWindow::GLWindow(wxWindow* parent, int* args) :
	wxGLCanvas(parent, wxID_ANY, args, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE) {
	m_context = new wxGLContext(this);
	SetSize(256,256);
	SetBackgroundStyle(wxBG_STYLE_CUSTOM);
	mouse_lefton = false;
	mouse_righton = false;
}

GLWindow::~GLWindow() {
	delete m_context;
}

void GLWindow::DisplayField(FieldTilesDataStruct* tiles, FieldWalkmeshDataStruct* walk) {
	GLint offx, offy, offz;
	unsigned int i;
	display_type = DISPLAY_GL_TYPE_FIELD;
	field_tiles = tiles;
	field_walk = walk;
	field_camera = 0;
	field_showtiles = false;
	field_showwalk = true;
	field_walk_path_highlight = -1;
	field_walk_triangle_highlight = -1;
	field_region_vertice_amount = 0;
	field_showpoint = 0;
	field_showplane = 0;
	field_walk_triangle_pos = new GLint*[3*field_walk->triangle_amount];
	for (i=0;i<field_walk->triangle_amount;i++) {
		offx = field_walk->offset_x+field_walk->walkpath_offsetx[field_walk->triangle_walkpath[i]];
		offy = field_walk->offset_y+field_walk->walkpath_offsety[field_walk->triangle_walkpath[i]];
		offz = -field_walk->offset_z-field_walk->walkpath_offsetz[field_walk->triangle_walkpath[i]];
		field_walk_triangle_pos[3*i] = new GLint[3];
		field_walk_triangle_pos[3*i+1] = new GLint[3];
		field_walk_triangle_pos[3*i+2] = new GLint[3];
		field_walk_triangle_pos[3*i][0] = offx+field_walk->vertice_x[field_walk->triangle_vertice1[i]];
		field_walk_triangle_pos[3*i][1] = offy+field_walk->vertice_y[field_walk->triangle_vertice1[i]];
		field_walk_triangle_pos[3*i][2] = offz-field_walk->vertice_z[field_walk->triangle_vertice1[i]];
		field_walk_triangle_pos[3*i+1][0] = offx+field_walk->vertice_x[field_walk->triangle_vertice2[i]];
		field_walk_triangle_pos[3*i+1][1] = offy+field_walk->vertice_y[field_walk->triangle_vertice2[i]];
		field_walk_triangle_pos[3*i+1][2] = offz-field_walk->vertice_z[field_walk->triangle_vertice2[i]];
		field_walk_triangle_pos[3*i+2][0] = offx+field_walk->vertice_x[field_walk->triangle_vertice3[i]];
		field_walk_triangle_pos[3*i+2][1] = offy+field_walk->vertice_y[field_walk->triangle_vertice3[i]];
		field_walk_triangle_pos[3*i+2][2] = offz-field_walk->vertice_z[field_walk->triangle_vertice3[i]];
	}
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
		uint32_t* backimg = field_tiles->ConvertAsImage(field_camera,tilesel,true);
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

void GLWindow::DisplayFieldRegion(unsigned int vertamount,int16_t* vert) {
	if (display_type!=DISPLAY_GL_TYPE_FIELD)
		return;
	DisplayFieldClear();
	unsigned int i,j;
	GLint zpos = -0x7FFF;
	field_region_vertice_amount = vertamount;
	field_region_vertice_pos = new GLint*[field_region_vertice_amount];
	for (i=0;i<vertamount;i++) {
		field_region_vertice_pos[i] = new GLint[3];
		field_region_vertice_pos[i][0] = vert[2*i];
		field_region_vertice_pos[i][1] = vert[2*i+1];
		for (j=0;j<field_walk->triangle_amount;j++)
			if (IsPointInTriangle2D(vert[2*i],vert[2*i+1],field_walk_triangle_pos[3*j][0],field_walk_triangle_pos[3*j][1],field_walk_triangle_pos[3*j+1][0],field_walk_triangle_pos[3*j+1][1],field_walk_triangle_pos[3*j+2][0],field_walk_triangle_pos[3*j+2][1])) {
				zpos = max(max(max(zpos,field_walk_triangle_pos[3*j][2]),field_walk_triangle_pos[3*j+1][2]),field_walk_triangle_pos[3*j+2][2])+DRAW_OVER_OFFSET;
				break;
			}
	}
	if (zpos==-0x7FFF)
		zpos = -field_walk->offset_z;
	for (i=0;i<vertamount;i++)
		field_region_vertice_pos[i][2] = zpos;
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
	if (display_type!=DISPLAY_GL_TYPE_FIELD)
		return;
	GLdouble* barpos, minbarpos, maxbarpos = DOUBLE_MIN_VALUE;
	unsigned int i;
	field_showpoint = 0;
	for (i=0;i<field_walk->triangle_amount;i++) {
		barpos = GetPointBarycenterCoef(x,y,field_walk_triangle_pos[3*i][0],field_walk_triangle_pos[3*i][1],field_walk_triangle_pos[3*i+1][0],field_walk_triangle_pos[3*i+1][1],field_walk_triangle_pos[3*i+2][0],field_walk_triangle_pos[3*i+2][1]);
		if (barpos) {
			minbarpos = min(min(barpos[0],barpos[1]),barpos[2]);
			if (minbarpos>=0 && field_showpoint<GL_WINDOW_FIELD_MAX_POINTS) {
				field_pointz[field_showpoint++] = barpos[0]*field_walk_triangle_pos[3*i][2]+barpos[1]*field_walk_triangle_pos[3*i+1][2]+barpos[2]*field_walk_triangle_pos[3*i+2][2]+DRAW_OVER_OFFSET;
			} else if (minbarpos>maxbarpos && field_showpoint==0) {
				if (max(max(barpos[0],barpos[1]),barpos[2])==barpos[0])
					field_pointz[0] = field_walk_triangle_pos[3*i][2]+DRAW_OVER_OFFSET;
				else if (max(max(barpos[0],barpos[1]),barpos[2])==barpos[1])
					field_pointz[0] = field_walk_triangle_pos[3*i+1][2]+DRAW_OVER_OFFSET;
				else
					field_pointz[0] = field_walk_triangle_pos[3*i+2][2]+DRAW_OVER_OFFSET;
				maxbarpos = minbarpos;
			}
		}
	}
	if (field_showpoint==0)
		field_showpoint = 1;
	for (i=0;i<field_showpoint;i++) {
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

void GLWindow::DisplayFieldClear() {
	if (display_type!=DISPLAY_GL_TYPE_FIELD)
		return;
	bool mustdraw = false;
	if (field_region_vertice_amount>0) {
		for (unsigned int i=0;i<field_region_vertice_amount;i++)
			delete[] field_region_vertice_pos[i];
		delete[] field_region_vertice_pos;
		field_region_vertice_amount = 0;
		mustdraw = true;
	}
	if (field_showpoint>0) {
		field_showpoint = 0;
		mustdraw = true;
	}
	if (field_showplane>0) {
		field_showplane = 0;
		mustdraw = true;
	}
	if (field_walk_triangle_highlight>=0) {
		field_walk_triangle_highlight = -1;
		mustdraw = true;
	}
	if (field_walk_path_highlight>=0) {
		field_walk_path_highlight = -1;
		mustdraw = true;
	}
	if (mustdraw)
		Draw();
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
	switch (display_type) {
	case DISPLAY_GL_TYPE_FIELD:
		camera_pos_x = (field_tiles->camera[field_camera].min_x+field_tiles->camera[field_camera].max_x)/2;
		camera_pos_y = -(field_tiles->camera[field_camera].min_y+field_tiles->camera[field_camera].max_y)/2;
		camera_pos_z = 0.0;
		camera_eye_x = field_tiles->camera[field_camera].eye_x;
		camera_eye_y = -field_tiles->camera[field_camera].eye_y;
		camera_eye_z = -field_tiles->camera[field_camera].eye_z;
		camera_angle_xy = atan2(camera_pos_y-camera_eye_y,camera_pos_x-camera_eye_x);
		camera_angle_z = atan2(camera_eye_z-camera_pos_z,sqrt((camera_pos_x-camera_eye_x)*(camera_pos_x-camera_eye_x)+(camera_pos_y-camera_eye_y)*(camera_pos_y-camera_eye_y)));
		break;
	default:
		camera_pos_x = 0;
		camera_pos_y = 0;
		camera_pos_z = 0.0;
		camera_eye_x = 0;
		camera_eye_y = 0;
		camera_eye_z = 1000.0;
		camera_angle_xy = M_PI/2;
		camera_angle_z = M_PI/2;
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
	glLoadIdentity();
	gluPerspective(65.0,1.0,1.0,100000.0L);
	gluLookAt(camera_eye_x,camera_eye_y,camera_eye_z,camera_pos_x,camera_pos_y,camera_pos_z,cos(camera_angle_xy)*sin(camera_angle_z),sin(camera_angle_xy)*sin(camera_angle_z),cos(camera_angle_z));
	if (display_type==DISPLAY_GL_TYPE_FIELD) {
		if (field_showtiles) {
			uint32_t* tileimg = field_tiles->ConvertAsImage(field_camera);
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
			for (i=0;i<field_walk->triangle_amount;i++) {
				if (field_walk->triangle_walkpath[i]==field_walk_path_highlight || i==field_walk_triangle_highlight)
					glColor4f(1, 0.9, 0, 1);
				else
					glColor4f(1, 0, 0, 1);
				glBegin(GL_TRIANGLES);
				glVertex3iv(field_walk_triangle_pos[3*i]);
				glVertex3iv(field_walk_triangle_pos[3*i+1]);
				glVertex3iv(field_walk_triangle_pos[3*i+2]);
				glEnd();
				glColor4f(0, 1, 0, 1);
				glBegin(GL_LINE_LOOP);
				glVertex3iv(field_walk_triangle_pos[3*i]);
				glVertex3iv(field_walk_triangle_pos[3*i+1]);
				glVertex3iv(field_walk_triangle_pos[3*i+2]);
				glEnd();
			}
		}
		if (field_region_vertice_amount>0) {
			glColor4f(0, 0, 1, 0.8);
			glBegin(GL_POLYGON);
			for (i=0;i<field_region_vertice_amount;i++)
				glVertex3iv(field_region_vertice_pos[i]);
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
			glColor4f(0, 0.3, 1, 0.5);
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
		GLdouble tz = 0;
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

bool IsPointInTriangle2D(GLdouble x,GLdouble y,GLdouble tx1,GLdouble ty1,GLdouble tx2,GLdouble ty2,GLdouble tx3,GLdouble ty3) {
	GLdouble det = (tx2-tx1)*(ty3-ty1)-(tx3-tx1)*(ty2-ty1);
	GLdouble s = (ty3-ty1)*(x-tx1)+(tx1-tx3)*(y-ty1);
	GLdouble t = (ty1-ty2)*(x-tx1)+(tx2-tx1)*(y-ty1);
	if (det<0)
		return s<=0 && s>=det && t<=0 && t>=det && s+t>=det;
	return s>=0 && s<=det && t>=0 && t<=det && s+t<=det;
}

GLdouble pointbarycoefresult[3];
GLdouble* GetPointBarycenterCoef(GLdouble x,GLdouble y,GLdouble tx1,GLdouble ty1,GLdouble tx2,GLdouble ty2,GLdouble tx3,GLdouble ty3) {
	GLdouble det = (tx2-tx1)*(ty3-ty1)-(tx3-tx1)*(ty2-ty1);
	if (det==0.0) // flat triangle
		return NULL;
	pointbarycoefresult[1] = ((ty3-ty1)*(x-tx1)+(tx1-tx3)*(y-ty1))/det;
	pointbarycoefresult[2] = ((ty1-ty2)*(x-tx1)+(tx2-tx1)*(y-ty1))/det;
	pointbarycoefresult[0] = 1-pointbarycoefresult[1]-pointbarycoefresult[2];
	return pointbarycoefresult;
}


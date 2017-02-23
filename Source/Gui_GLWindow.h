#ifndef _GUI_GLWINDOW_H
#define _GUI_GLWINDOW_H

class GLWindow;

#include <wx/wx.h>
#include <wx/glcanvas.h>
#include "Fields.h"

#define GL_WINDOW_FIELD_MAX_POINTS	20

#define DISPLAY_GL_TYPE_FIELD	1

class GLWindow : public wxGLCanvas {
public:
	FieldTilesDataStruct* field_tiles;
	FieldWalkmeshDataStruct* field_walk;
	unsigned int field_camera;
	bool field_showtiles;
	bool field_showwalk;
	
	GLint** field_walk_triangle_pos;
	int field_walk_path_highlight;
	int field_walk_triangle_highlight;
	unsigned int field_region_vertice_amount;
	GLint** field_region_vertice_pos;
	unsigned int field_showpoint;
	GLint field_pointx[GL_WINDOW_FIELD_MAX_POINTS];
	GLint field_pointy[GL_WINDOW_FIELD_MAX_POINTS];
	GLint field_pointz[GL_WINDOW_FIELD_MAX_POINTS];
	unsigned int field_showplane;
	GLint field_plane_coord;
	
	GLint*** field_tiles_quad_pos;
	int field_tiles_highlight;
	
	unsigned int display_type;
	GLfloat camera_eye_x;
	GLfloat camera_eye_y;
	GLfloat camera_eye_z;
	GLfloat camera_pos_x;
	GLfloat camera_pos_y;
	GLfloat camera_pos_z;
	GLfloat camera_angle_xy;
	GLfloat camera_angle_z;
	bool mouse_lefton;
	bool mouse_righton;
	int mouse_x;
	int mouse_y;
	GLuint* texture_id;
	
	void DisplayField(FieldTilesDataStruct* tiles, FieldWalkmeshDataStruct* walk);
	void DisplayFieldRegion(unsigned int vertamount, int16_t* vert);
	void DisplayFieldPoint3D(int16_t x, int16_t y, int16_t z);
	void DisplayFieldPoint2D(int16_t x, int16_t y);
	void DisplayFieldPlane(int planetype, int16_t coord); // 1 for x ; 2 for y ; 3 for z
	void DisplayFieldTrianglePath(int triangleid);
	void DisplayFieldPath(int pathid);
	void DisplayFieldClear();
	
	void Draw();
	void ResetCamera();
	void Prepare3DViewport(int topleft_x, int topleft_y, int bottomrigth_x, int bottomrigth_y);
	void Prepare2DViewport(int topleft_x, int topleft_y, int bottomrigth_x, int bottomrigth_y);
	GLWindow(wxWindow* parent, int* args);
	virtual ~GLWindow();
	
private:
    wxGLContext* m_context;
	
	void OnPaint(wxPaintEvent& evt);
	void OnResize(wxSizeEvent& evt);
	void OnMouseMove(wxMouseEvent& event);
	void OnMouseWheel(wxMouseEvent& event);
	void OnLeftClick(wxMouseEvent& event);
	void OnLeftRelease(wxMouseEvent& event);
	void OnRightClick(wxMouseEvent& event);
	void OnRightRelease(wxMouseEvent& event);
	void OnMouseLeaveWindow(wxMouseEvent& event);
	void OnKeyPress(wxKeyEvent& event);
	
	DECLARE_EVENT_TABLE()
};

#endif
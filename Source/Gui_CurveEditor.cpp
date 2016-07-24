#include "Gui_CurveEditor.h"

#include "Gui_TextureEditor.h"
#include "main.h"

#define DEFAULT_WINSIZE_X 290
#define DEFAULT_WINSIZE_Y 290
#define MAX_WINSIZE_X 10000
#define MAX_WINSIZE_Y 10000
using namespace std;

CurveEditorDialog::CurveEditorDialog(wxWindow* parent) :
	CurveEditorWindow(parent),
	win_size_x(DEFAULT_WINSIZE_X),
	win_size_y(DEFAULT_WINSIZE_Y),
	last_vali(0) {
}

CurveEditorDialog::~CurveEditorDialog() {
	delete[] new_x;
	delete[] new_y;
}

int CurveEditorDialog::ShowModal(unsigned int amount, uint32_t* xval, uint32_t* yval) {
	unsigned int i;
	init_x = xval;
	init_y = yval;
	val_amount = amount;
	new_x = new uint32_t[val_amount];
	new_y = new uint32_t[val_amount];
	memcpy(new_x,init_x,val_amount*4);
	memcpy(new_y,init_y,val_amount*4);
	max_x = init_x[val_amount-1];
	max_y = 0;
	for (i=0;i<val_amount;i++)
		if (init_y[i]>max_y)
			max_y = init_y[i];
	m_curvescrolled->SetVirtualSize(win_size_x,win_size_y+10);
	wxClientDC dc(m_curvescrolled);
	DrawImage(dc);
	return wxDialog::ShowModal();
}

wxPoint CurveEditorDialog::ConvertCoordToPoint(uint32_t x, uint32_t y) {
	return wxPoint(double(x)/max_x*win_size_x,win_size_y-double(y)/max_y*win_size_y);
}

wxPoint* ptarray = NULL;
wxPoint* CurveEditorDialog::ConvertArrayToPoints(int amount, uint32_t* arrayx, uint32_t* arrayy) {
	if (ptarray)
		delete[] ptarray;
	ptarray = new wxPoint[amount];
	for (int i=0;i<amount;i++)
		ptarray[i] = ConvertCoordToPoint(arrayx[i],arrayy[i]);
	return ptarray;
}

void CurveEditorDialog::DrawImage(wxDC& dc, wxPoint displaycoord, wxString coordstr) {
	unsigned int i;
	MACRO_CREATE_MEMORY_DC(dc,m_curvescrolled)
	wxPen blackpen = *wxBLACK_PEN;
	wxPen greypen = *wxGREY_PEN;
	mdc.SetPen(blackpen);
	if (m_linkup->IsChecked()) {
		mdc.SetPen(greypen);
		mdc.DrawLines(val_amount,ConvertArrayToPoints(val_amount,init_x,init_y));
		mdc.SetPen(blackpen);
		mdc.DrawLines(val_amount,ConvertArrayToPoints(val_amount,new_x,new_y));
	} else
		for (i=0;i<val_amount;i++) {
			mdc.SetPen(greypen);
			mdc.DrawPoint(ConvertCoordToPoint(init_x[i],init_y[i]));
			mdc.SetPen(blackpen);
			mdc.DrawPoint(ConvertCoordToPoint(new_x[i],new_y[i]));
		}
	if (displaycoord!=wxDefaultPosition)
		mdc.DrawText(coordstr,displaycoord);
	m_curvescrolled->DoPrepareDC(dc);
	dc.Blit(wxPoint(0,0),mdc.GetSize(),&mdc,wxPoint(0,0));
}

void CurveEditorDialog::OnMouse(wxMouseEvent& event) {
	if (event.LeftIsDown()) {
		wxClientDC dc(m_curvescrolled);
		m_curvescrolled->DoPrepareDC(dc);
		wxPoint mpos = event.GetLogicalPosition(dc);
		if (mpos.x>=0 && mpos.x<=win_size_x && mpos.y>=0 && mpos.y<=win_size_y) {
			int imgx = mpos.x*max_x/win_size_x;
			unsigned int vali=0;
			while (vali<val_amount && imgx>(int)new_x[vali]) vali++;
			vali = vali==val_amount ? vali-1 : vali;
			if (event.ShiftDown()) {
				if (last_vali!=vali) {
					unsigned int firsti = min(last_vali,vali);
					unsigned int lasti = max(last_vali,vali);
					uint32_t newyfirst = last_vali<vali ? new_y[last_vali] : max_y-double(mpos.y)/win_size_y*max_y;
					uint32_t newylast = last_vali>vali ? new_y[last_vali] : max_y-double(mpos.y)/win_size_y*max_y;
					int diff = lasti-firsti;
					for (int i=0;i<=diff;i++)
						new_y[firsti+i] = round(double(((diff-i)*newyfirst+i*newylast)/diff));
				}
			} else {
				new_y[vali] = max_y-double(mpos.y)/win_size_y*max_y;
				last_vali = vali;
			}
			wxCoord txtx, txty;
			wxString txt;
			txt.Printf(wxT("( %ld, %ld )"),new_x[vali],new_y[vali]);
			wxSize txtsize = dc.GetTextExtent(txt);
			if (event.GetX()>=DEFAULT_WINSIZE_X-txtsize.GetWidth())
				txtx = mpos.x-txtsize.GetWidth();
			else
				txtx = mpos.x;
			if (event.GetY()<=txtsize.GetHeight())
				txty = mpos.y;
			else
				txty = mpos.y-txtsize.GetHeight();
			wxClientDC drawdc(m_curvescrolled);
			DrawImage(drawdc,wxPoint(txtx,txty),txt);
		}
	} else if (event.LeftUp()) {
		wxClientDC drawdc(m_curvescrolled);
		DrawImage(drawdc);
	}
}

void CurveEditorDialog::OnMouseWheel(wxMouseEvent& event) {
	double delta = 0.1*double(event.GetWheelRotation())/event.GetWheelDelta();
	win_size_x = min(MAX_WINSIZE_X,(max(DEFAULT_WINSIZE_X,int(win_size_x*(1+delta)))));
	win_size_y = min(MAX_WINSIZE_Y,(max(DEFAULT_WINSIZE_Y,int(win_size_y*(1+delta)))));
	m_curvescrolled->SetVirtualSize(win_size_x,win_size_y+10);
	wxClientDC dc(m_curvescrolled);
	DrawImage(dc);
}

void CurveEditorDialog::OnPaint(wxPaintEvent& event) {
	wxPaintDC dc(m_curvescrolled);
	DrawImage(dc);
}

void CurveEditorDialog::OnCheckBox(wxCommandEvent& event) {
	wxClientDC dc(m_curvescrolled);
	DrawImage(dc);
}

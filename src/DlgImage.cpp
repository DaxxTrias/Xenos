#include "DlgImage.h"

ImageDlg::ImageDlg( ImageContext& Image )
	: Dialog( IDD_IMAGE )
	, _image( Image )
{
	_messages[WM_INITDIALOG]	= static_cast<Dialog::fnDlgProc>(&ImageDlg::OnInit);

	_events[IDC_REMOVE_CALL]	= static_cast<Dialog::fnDlgProc>(&ImageDlg::OnRemove);
	_events[IDC_ADD_CALL]		= static_cast<Dialog::fnDlgProc>(&ImageDlg::OnAdd);
	_events[ID_IMAGE_OK]		= static_cast<Dialog::fnDlgProc>(&ImageDlg::OnOk);
	_events[ID_IMAGE_CANCEL]	= static_cast<Dialog::fnDlgProc>(&ImageDlg::OnCancel);

	_image.GetExports( _exports );
	_routines = _image.initRoutines;
}

INT_PTR ImageDlg::OnInit( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam )
{
	Dialog::OnInit( hDlg, message, wParam, lParam );

	_initList.Attach( _hwnd, IDC_IMAGE_CALLS );
	_expCombo.Attach( _hwnd, IDC_IMAGE_EXPS );
	_initArg.Attach( _hwnd, IDC_CALL_ARG );

	ListView_SetExtendedListViewStyle( _initList.hwnd(), LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER );

	//
	// Insert columns
	//
	_initList.AddColumn( L"Name", 100, Name );
	_initList.AddColumn( L"Args", 220, Args );

	// List existing init routines
	for (auto& call : _routines)
	{
		std::wstring wstr = std::wstring( call.exp.name.size(), 'a' );
		for (size_t i = 0; i < wstr.size(); i++)
			wstr[i] = (wchar_t)call.exp.name[i];
		
		_initList.AddItem( wstr, 0, { call.args } );
	}

	// List exports
	_expCombo.reset();
	for (auto& exp : _exports)
		_expCombo.Add( exp.name );

	if (_exports.size())
		ComboBox_SetCurSel( _expCombo.hwnd(), 0 );

	return TRUE;
}

INT_PTR ImageDlg::OnRemove( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam )
{
	int selection = _initList.selection();
	if (selection >= 0)
	{
		_initList.RemoveItem( selection );
		_routines.erase(_routines.begin() + selection);
	}

	return TRUE;
}

INT_PTR ImageDlg::OnAdd( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam )
{
	int selection = _expCombo.selection();
	if (selection >= 0)
	{
		_initList.AddItem( _expCombo.selectedText(), 0, { _initArg.text() } );
		_routines.push_back( { _exports[selection], _initArg.text() } );
	}

	return TRUE;
}

INT_PTR ImageDlg::OnOk( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam )
{
	_image.initRoutines = _routines;
	return CloseDialog();
}

INT_PTR ImageDlg::OnCancel( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam )
{
	return CloseDialog();
}
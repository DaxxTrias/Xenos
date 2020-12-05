#pragma once

#include "Dialog.hpp"
#include "resource.h"
#include "ListView.hpp"
#include "ComboBox.hpp"
#include "EditBox.hpp"
#include "InjectionCore.h"

class ImageDlg : public Dialog
{
	enum ColumnID
	{
		Name = 0,
		Args
	};

public:
	ImageDlg( ImageContext& Image );
	~ImageDlg() { }

private:

	MSG_HANDLER( OnInit );
	MSG_HANDLER( OnAdd );
	MSG_HANDLER( OnRemove );
	MSG_HANDLER( OnOk );
	MSG_HANDLER( OnCancel) ;

private:
	blackbone::pe::vecExports _exports;
	vecRoutines _routines;

	ctrl::ListView _initList;
	ctrl::ComboBox _expCombo;
	ctrl::EditBox _initArg;
	ImageContext& _image;
};
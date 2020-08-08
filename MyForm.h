#pragma once
#include <sstream>
#include <string>
#include <iostream>
#include <opencv\highgui.h>
#include <opencv\cv.h>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "about.h"

namespace RDsystem {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace cv;
	using namespace std;

	//define variable for selected view number.
	int viewSelector;
	
	//define variables for structuring element of "dilate" and "erode" image.
	int p;
	int q;
	int r;
	int s;
	
	//define variables for bouding rectangle colors.
	int R = 0;
	int G = 255;
	int B = 0;

	//define min and max HSV filter values.
	int H_MIN;
	int H_MAX;
	int S_MIN;
	int S_MAX;
	int V_MIN;
	int V_MAX;
	
	//matrix storage for HSV image
	Mat HSV;
	
	//matrix storage for binary threshold image
	Mat threshold;
	
	//matrix storage for the input image
	Mat inputImage;
	
	//matrix storage for the output image
	Mat output;
	
	//matrix storage for the temperary
	Mat temp;
	
	//matrix storage for the contours
	Mat drawing;
	
	//These names that will appear at the top of each window
	const string windowName = "Californiya Institute of Technology";
	const string windowName1 = "HSV Image";
	const string windowName2 = "Thresholded Image";
	const string windowName3 = "Contours Image";
	
	//this function for set values for HSV filter
	void setValues(int a, int b, int c, int d, int e, int f) {
		H_MIN = a;
		S_MIN = b;
		V_MIN = c;
		H_MAX = d;
		S_MAX = e;
		V_MAX = f;
	}
	
	//morphological operations used to "dilate" and "erode" the image.
	void morphOps(Mat &thresh) {
		// p, q, r, s values are get by according to the view and the object.
		Mat erodeElement = getStructuringElement(MORPH_RECT, cv::Size(p, q));
		
		Mat dilateElement = getStructuringElement(MORPH_RECT, cv::Size(r, s));

		erode(thresh, thresh, erodeElement);
		dilate(thresh, thresh, erodeElement);

		dilate(thresh, thresh, dilateElement);
		erode(thresh, thresh, dilateElement);
	}
	
	//by this function, draw contours, draw bounding rects and much more...
	void mainProgram()
	{
		//filter HSV image between values and store filtered image to threshold matrix
		inRange(HSV, Scalar(H_MIN, S_MIN, V_MIN), Scalar(H_MAX, S_MAX, V_MAX), threshold);
		
		//perform morphological operations on thresholded image to eliminate noise and emphasize the filtered object
		morphOps(threshold);
		
		//copying threshold to temperary matrix
		threshold.copyTo(temp);
		
		//these two vectors needed for output of findContours
		vector< vector<cv::Point> > contours;
		vector<Vec4i> hierarchy;
		
		//find contours of filtered image using openCV findContours function
		findContours(temp, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
		
		// Approximate contours to polygons and get bounding rects
		vector<vector<cv::Point> > contours_poly(contours.size());
		vector<Rect> boundRect(contours.size());
		
		//loop withing contours
		for (size_t i = 0; i < contours.size(); i++)
		{
			approxPolyDP(contours[i], contours_poly[i], 3, true);
			boundRect[i] = boundingRect(contours_poly[i]);
		}
		
		//use another matrix for contours, and get the threshold image size
		drawing = Mat::zeros(threshold.size(), CV_8UC3);
		
		//copy original image to another matrix to apply bounding rects
		inputImage.copyTo(output);
		
		//loop between contours to draw bounding rects
		for (size_t i = 0; i< contours.size(); i++)
		{
			Scalar color = Scalar(B, G, R);
			drawContours(drawing, contours_poly, (int)i, color, 1, 8, vector<Vec4i>(), 0, cv::Point());
			rectangle(output, boundRect[i].tl(), boundRect[i].br(), color, 3, 8, 0);
		}

		//show the output image with bounding rectangles
		imshow(windowName, output);	
	}

	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}
	
	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::MenuStrip^  menuStrip1;
	protected:
	private: System::Windows::Forms::ToolStripMenuItem^  fileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  editToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  optionsToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  helpToolStripMenuItem;
	private: System::Windows::Forms::GroupBox^  groupBox1;
	private: System::Windows::Forms::ComboBox^  comboBox1;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::GroupBox^  groupBox2;
	private: System::Windows::Forms::RadioButton^  radioButton3;
	private: System::Windows::Forms::RadioButton^  radioButton5;
	private: System::Windows::Forms::RadioButton^  radioButton4;
	private: System::Windows::Forms::RadioButton^  radioButton2;
	private: System::Windows::Forms::RadioButton^  radioButton1;
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::ToolStripMenuItem^  viewToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  view01ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  view02ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  view03ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  regionsToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  athleticFieldToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  innerGroundToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  tennisCourtsToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  mainGroundToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  astroscienceLaboratoryToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  changeColorToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  redToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  greenToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  blueToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  showWindowsToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  toolStripMenuItem1;
	private: System::Windows::Forms::ToolStripMenuItem^  thresholdToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  contoursToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  exitToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  grayToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  yellowToolStripMenuItem;


	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->viewToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->view01ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->view02ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->view03ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->regionsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->athleticFieldToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->innerGroundToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->tennisCourtsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mainGroundToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->astroscienceLaboratoryToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->exitToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->editToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->changeColorToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->redToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->greenToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->blueToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->grayToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->yellowToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->optionsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->showWindowsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->thresholdToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->contoursToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->helpToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->comboBox1 = (gcnew System::Windows::Forms::ComboBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
			this->radioButton3 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButton5 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButton4 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButton2 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButton1 = (gcnew System::Windows::Forms::RadioButton());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->menuStrip1->SuspendLayout();
			this->groupBox1->SuspendLayout();
			this->groupBox2->SuspendLayout();
			this->SuspendLayout();
			// 
			// menuStrip1
			// 
			this->menuStrip1->BackColor = System::Drawing::SystemColors::ActiveCaption;
			this->menuStrip1->ImageScalingSize = System::Drawing::Size(20, 20);
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {
				this->fileToolStripMenuItem,
					this->editToolStripMenuItem, this->optionsToolStripMenuItem, this->helpToolStripMenuItem
			});
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Size = System::Drawing::Size(569, 28);
			this->menuStrip1->TabIndex = 0;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// fileToolStripMenuItem
			// 
			this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {
				this->viewToolStripMenuItem,
					this->regionsToolStripMenuItem, this->exitToolStripMenuItem
			});
			this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
			this->fileToolStripMenuItem->Size = System::Drawing::Size(44, 24);
			this->fileToolStripMenuItem->Text = L"File";
			// 
			// viewToolStripMenuItem
			// 
			this->viewToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {
				this->view01ToolStripMenuItem,
					this->view02ToolStripMenuItem, this->view03ToolStripMenuItem
			});
			this->viewToolStripMenuItem->Name = L"viewToolStripMenuItem";
			this->viewToolStripMenuItem->Size = System::Drawing::Size(137, 26);
			this->viewToolStripMenuItem->Text = L"View";
			// 
			// view01ToolStripMenuItem
			// 
			this->view01ToolStripMenuItem->Name = L"view01ToolStripMenuItem";
			this->view01ToolStripMenuItem->Size = System::Drawing::Size(136, 26);
			this->view01ToolStripMenuItem->Text = L"View 01";
			this->view01ToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::view01ToolStripMenuItem_Click);
			// 
			// view02ToolStripMenuItem
			// 
			this->view02ToolStripMenuItem->Name = L"view02ToolStripMenuItem";
			this->view02ToolStripMenuItem->Size = System::Drawing::Size(136, 26);
			this->view02ToolStripMenuItem->Text = L"View 02";
			this->view02ToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::view02ToolStripMenuItem_Click);
			// 
			// view03ToolStripMenuItem
			// 
			this->view03ToolStripMenuItem->Name = L"view03ToolStripMenuItem";
			this->view03ToolStripMenuItem->Size = System::Drawing::Size(136, 26);
			this->view03ToolStripMenuItem->Text = L"View 03";
			this->view03ToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::view03ToolStripMenuItem_Click);
			// 
			// regionsToolStripMenuItem
			// 
			this->regionsToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(5) {
				this->athleticFieldToolStripMenuItem,
					this->innerGroundToolStripMenuItem, this->tennisCourtsToolStripMenuItem, this->mainGroundToolStripMenuItem, this->astroscienceLaboratoryToolStripMenuItem
			});
			this->regionsToolStripMenuItem->Name = L"regionsToolStripMenuItem";
			this->regionsToolStripMenuItem->Size = System::Drawing::Size(137, 26);
			this->regionsToolStripMenuItem->Text = L"Regions";
			// 
			// athleticFieldToolStripMenuItem
			// 
			this->athleticFieldToolStripMenuItem->Name = L"athleticFieldToolStripMenuItem";
			this->athleticFieldToolStripMenuItem->Size = System::Drawing::Size(243, 26);
			this->athleticFieldToolStripMenuItem->Text = L"Athletic Field";
			this->athleticFieldToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::athleticFieldToolStripMenuItem_Click);
			// 
			// innerGroundToolStripMenuItem
			// 
			this->innerGroundToolStripMenuItem->Name = L"innerGroundToolStripMenuItem";
			this->innerGroundToolStripMenuItem->Size = System::Drawing::Size(243, 26);
			this->innerGroundToolStripMenuItem->Text = L"Inner Ground";
			this->innerGroundToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::innerGroundToolStripMenuItem_Click);
			// 
			// tennisCourtsToolStripMenuItem
			// 
			this->tennisCourtsToolStripMenuItem->Name = L"tennisCourtsToolStripMenuItem";
			this->tennisCourtsToolStripMenuItem->Size = System::Drawing::Size(243, 26);
			this->tennisCourtsToolStripMenuItem->Text = L"Tennis Courts";
			this->tennisCourtsToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::tennisCourtsToolStripMenuItem_Click);
			// 
			// mainGroundToolStripMenuItem
			// 
			this->mainGroundToolStripMenuItem->Name = L"mainGroundToolStripMenuItem";
			this->mainGroundToolStripMenuItem->Size = System::Drawing::Size(243, 26);
			this->mainGroundToolStripMenuItem->Text = L"Main Ground";
			this->mainGroundToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::mainGroundToolStripMenuItem_Click);
			// 
			// astroscienceLaboratoryToolStripMenuItem
			// 
			this->astroscienceLaboratoryToolStripMenuItem->Name = L"astroscienceLaboratoryToolStripMenuItem";
			this->astroscienceLaboratoryToolStripMenuItem->Size = System::Drawing::Size(243, 26);
			this->astroscienceLaboratoryToolStripMenuItem->Text = L"Astroscience Laboratory";
			this->astroscienceLaboratoryToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::astroscienceLaboratoryToolStripMenuItem_Click);
			// 
			// exitToolStripMenuItem
			// 
			this->exitToolStripMenuItem->Name = L"exitToolStripMenuItem";
			this->exitToolStripMenuItem->Size = System::Drawing::Size(137, 26);
			this->exitToolStripMenuItem->Text = L"Exit";
			this->exitToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::exitToolStripMenuItem_Click);
			// 
			// editToolStripMenuItem
			// 
			this->editToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->changeColorToolStripMenuItem });
			this->editToolStripMenuItem->Name = L"editToolStripMenuItem";
			this->editToolStripMenuItem->Size = System::Drawing::Size(47, 24);
			this->editToolStripMenuItem->Text = L"Edit";
			// 
			// changeColorToolStripMenuItem
			// 
			this->changeColorToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(5) {
				this->redToolStripMenuItem,
					this->greenToolStripMenuItem, this->blueToolStripMenuItem, this->grayToolStripMenuItem, this->yellowToolStripMenuItem
			});
			this->changeColorToolStripMenuItem->Name = L"changeColorToolStripMenuItem";
			this->changeColorToolStripMenuItem->Size = System::Drawing::Size(174, 26);
			this->changeColorToolStripMenuItem->Text = L"Change Color";
			// 
			// redToolStripMenuItem
			// 
			this->redToolStripMenuItem->Name = L"redToolStripMenuItem";
			this->redToolStripMenuItem->Size = System::Drawing::Size(143, 26);
			this->redToolStripMenuItem->Text = L"Red";
			this->redToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::redToolStripMenuItem_Click);
			// 
			// greenToolStripMenuItem
			// 
			this->greenToolStripMenuItem->Name = L"greenToolStripMenuItem";
			this->greenToolStripMenuItem->Size = System::Drawing::Size(143, 26);
			this->greenToolStripMenuItem->Text = L"Green";
			this->greenToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::greenToolStripMenuItem_Click);
			// 
			// blueToolStripMenuItem
			// 
			this->blueToolStripMenuItem->Name = L"blueToolStripMenuItem";
			this->blueToolStripMenuItem->Size = System::Drawing::Size(143, 26);
			this->blueToolStripMenuItem->Text = L"Blue";
			this->blueToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::blueToolStripMenuItem_Click);
			// 
			// grayToolStripMenuItem
			// 
			this->grayToolStripMenuItem->Name = L"grayToolStripMenuItem";
			this->grayToolStripMenuItem->Size = System::Drawing::Size(143, 26);
			this->grayToolStripMenuItem->Text = L"Magenta";
			this->grayToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::grayToolStripMenuItem_Click);
			// 
			// yellowToolStripMenuItem
			// 
			this->yellowToolStripMenuItem->Name = L"yellowToolStripMenuItem";
			this->yellowToolStripMenuItem->Size = System::Drawing::Size(143, 26);
			this->yellowToolStripMenuItem->Text = L"Yellow";
			this->yellowToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::yellowToolStripMenuItem_Click);
			// 
			// optionsToolStripMenuItem
			// 
			this->optionsToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->showWindowsToolStripMenuItem });
			this->optionsToolStripMenuItem->Name = L"optionsToolStripMenuItem";
			this->optionsToolStripMenuItem->Size = System::Drawing::Size(87, 24);
			this->optionsToolStripMenuItem->Text = L"Advanced";
			// 
			// showWindowsToolStripMenuItem
			// 
			this->showWindowsToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {
				this->toolStripMenuItem1,
					this->thresholdToolStripMenuItem, this->contoursToolStripMenuItem
			});
			this->showWindowsToolStripMenuItem->Name = L"showWindowsToolStripMenuItem";
			this->showWindowsToolStripMenuItem->Size = System::Drawing::Size(120, 26);
			this->showWindowsToolStripMenuItem->Text = L"Show";
			// 
			// toolStripMenuItem1
			// 
			this->toolStripMenuItem1->Name = L"toolStripMenuItem1";
			this->toolStripMenuItem1->Size = System::Drawing::Size(149, 26);
			this->toolStripMenuItem1->Text = L"HSV";
			this->toolStripMenuItem1->Click += gcnew System::EventHandler(this, &MyForm::toolStripMenuItem1_Click);
			// 
			// thresholdToolStripMenuItem
			// 
			this->thresholdToolStripMenuItem->Name = L"thresholdToolStripMenuItem";
			this->thresholdToolStripMenuItem->Size = System::Drawing::Size(149, 26);
			this->thresholdToolStripMenuItem->Text = L"Threshold";
			this->thresholdToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::thresholdToolStripMenuItem_Click);
			// 
			// contoursToolStripMenuItem
			// 
			this->contoursToolStripMenuItem->Name = L"contoursToolStripMenuItem";
			this->contoursToolStripMenuItem->Size = System::Drawing::Size(149, 26);
			this->contoursToolStripMenuItem->Text = L"Contours";
			this->contoursToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::contoursToolStripMenuItem_Click);
			// 
			// helpToolStripMenuItem
			// 
			this->helpToolStripMenuItem->Name = L"helpToolStripMenuItem";
			this->helpToolStripMenuItem->Size = System::Drawing::Size(62, 24);
			this->helpToolStripMenuItem->Text = L"About";
			this->helpToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::helpToolStripMenuItem_Click);
			// 
			// groupBox1
			// 
			this->groupBox1->BackColor = System::Drawing::SystemColors::GradientActiveCaption;
			this->groupBox1->Controls->Add(this->comboBox1);
			this->groupBox1->Controls->Add(this->label1);
			this->groupBox1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->groupBox1->Location = System::Drawing::Point(13, 47);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(544, 93);
			this->groupBox1->TabIndex = 1;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"View";
			// 
			// comboBox1
			// 
			this->comboBox1->BackColor = System::Drawing::SystemColors::InactiveCaption;
			this->comboBox1->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->comboBox1->FormattingEnabled = true;
			this->comboBox1->ItemHeight = 18;
			this->comboBox1->Items->AddRange(gcnew cli::array< System::Object^  >(3) { L"View 01", L"View 02", L"View 03" });
			this->comboBox1->Location = System::Drawing::Point(267, 41);
			this->comboBox1->Name = L"comboBox1";
			this->comboBox1->Size = System::Drawing::Size(205, 26);
			this->comboBox1->TabIndex = 1;
			this->comboBox1->SelectedIndexChanged += gcnew System::EventHandler(this, &MyForm::comboBox1_SelectedIndexChanged);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(16, 44);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(214, 18);
			this->label1->TabIndex = 0;
			this->label1->Text = L"Select Your Prefered View :";
			// 
			// groupBox2
			// 
			this->groupBox2->BackColor = System::Drawing::SystemColors::GradientActiveCaption;
			this->groupBox2->Controls->Add(this->radioButton3);
			this->groupBox2->Controls->Add(this->radioButton5);
			this->groupBox2->Controls->Add(this->radioButton4);
			this->groupBox2->Controls->Add(this->radioButton2);
			this->groupBox2->Controls->Add(this->radioButton1);
			this->groupBox2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->groupBox2->Location = System::Drawing::Point(13, 159);
			this->groupBox2->Name = L"groupBox2";
			this->groupBox2->Size = System::Drawing::Size(544, 169);
			this->groupBox2->TabIndex = 2;
			this->groupBox2->TabStop = false;
			this->groupBox2->Text = L"Regions";
			// 
			// radioButton3
			// 
			this->radioButton3->AutoSize = true;
			this->radioButton3->Location = System::Drawing::Point(19, 128);
			this->radioButton3->Name = L"radioButton3";
			this->radioButton3->Size = System::Drawing::Size(135, 22);
			this->radioButton3->TabIndex = 0;
			this->radioButton3->TabStop = true;
			this->radioButton3->Text = L"Tennis Courts";
			this->radioButton3->UseVisualStyleBackColor = true;
			this->radioButton3->CheckedChanged += gcnew System::EventHandler(this, &MyForm::radioButton3_CheckedChanged);
			// 
			// radioButton5
			// 
			this->radioButton5->AutoSize = true;
			this->radioButton5->Location = System::Drawing::Point(267, 82);
			this->radioButton5->Name = L"radioButton5";
			this->radioButton5->Size = System::Drawing::Size(213, 22);
			this->radioButton5->TabIndex = 0;
			this->radioButton5->TabStop = true;
			this->radioButton5->Text = L"Astroscience Laboratory";
			this->radioButton5->UseVisualStyleBackColor = true;
			this->radioButton5->CheckedChanged += gcnew System::EventHandler(this, &MyForm::radioButton5_CheckedChanged);
			// 
			// radioButton4
			// 
			this->radioButton4->AutoSize = true;
			this->radioButton4->Location = System::Drawing::Point(267, 41);
			this->radioButton4->Name = L"radioButton4";
			this->radioButton4->Size = System::Drawing::Size(126, 22);
			this->radioButton4->TabIndex = 0;
			this->radioButton4->TabStop = true;
			this->radioButton4->Text = L"Main Ground";
			this->radioButton4->UseVisualStyleBackColor = true;
			this->radioButton4->CheckedChanged += gcnew System::EventHandler(this, &MyForm::radioButton4_CheckedChanged);
			// 
			// radioButton2
			// 
			this->radioButton2->AutoSize = true;
			this->radioButton2->Location = System::Drawing::Point(19, 82);
			this->radioButton2->Name = L"radioButton2";
			this->radioButton2->Size = System::Drawing::Size(127, 22);
			this->radioButton2->TabIndex = 0;
			this->radioButton2->TabStop = true;
			this->radioButton2->Text = L"Inner Ground";
			this->radioButton2->UseVisualStyleBackColor = true;
			this->radioButton2->CheckedChanged += gcnew System::EventHandler(this, &MyForm::radioButton2_CheckedChanged);
			// 
			// radioButton1
			// 
			this->radioButton1->AutoSize = true;
			this->radioButton1->Location = System::Drawing::Point(19, 41);
			this->radioButton1->Name = L"radioButton1";
			this->radioButton1->Size = System::Drawing::Size(125, 22);
			this->radioButton1->TabIndex = 0;
			this->radioButton1->TabStop = true;
			this->radioButton1->Text = L"Athletic Field";
			this->radioButton1->UseVisualStyleBackColor = true;
			this->radioButton1->CheckedChanged += gcnew System::EventHandler(this, &MyForm::radioButton1_CheckedChanged);
			// 
			// button1
			// 
			this->button1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button1->Location = System::Drawing::Point(228, 353);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 29);
			this->button1->TabIndex = 3;
			this->button1->Text = L"EXIT";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &MyForm::button1_Click);
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::SystemColors::GradientInactiveCaption;
			this->ClientSize = System::Drawing::Size(569, 395);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->groupBox2);
			this->Controls->Add(this->groupBox1);
			this->Controls->Add(this->menuStrip1);
			this->MainMenuStrip = this->menuStrip1;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"MyForm";
			this->ShowIcon = false;
			this->Text = L"RDsystem";
			this->TopMost = true;
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			this->groupBox2->ResumeLayout(false);
			this->groupBox2->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
		
#pragma endregion
	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
		//show a message to confirm
		if (MessageBox::Show("Are you sure want to exit?", "Warning!", MessageBoxButtons::YesNo, MessageBoxIcon::Warning) == Windows::Forms::DialogResult::Yes) {
			destroyAllWindows();
			Application::Exit();
		}
	}
	
private: System::Void comboBox1_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
	//regions are disabled until select a view
	groupBox2->Enabled = true;
	
	//get the selected view
	viewSelector = comboBox1->SelectedIndex;
	
	switch (viewSelector) {
	case 0:
		//destroy all opened windows before excute
		destroyAllWindows();
		//uncheck all radio buttons
		radioButton1->Checked = false;
		radioButton2->Checked = false;
		radioButton3->Checked = false;
		radioButton4->Checked = false;
		radioButton5->Checked = false;
		
		//open image
		inputImage = imread("view1.png");
		
		//convert image from BGR to HSV colorspace
		cvtColor(inputImage, HSV, COLOR_BGR2HSV);
		
		//show the image
		imshow(windowName, inputImage);
		
		break;
		
	case 1:
		//destroy all opened windows before excute
		destroyAllWindows();
		//uncheck all radio buttons
		radioButton1->Checked = false;
		radioButton2->Checked = false;
		radioButton3->Checked = false;
		radioButton4->Checked = false;
		radioButton5->Checked = false;
		//open image
		inputImage = imread("view2.png"); 
		//convert image from BGR to HSV colorspace
		cvtColor(inputImage, HSV, COLOR_BGR2HSV);
		//show the image
		imshow(windowName, inputImage);
		break;
		
	case 2:
		//destroy all opened windows before excute
		destroyAllWindows();
		//uncheck all radio buttons
		radioButton1->Checked = false;
		radioButton2->Checked = false;
		radioButton3->Checked = false;
		radioButton4->Checked = false;
		radioButton5->Checked = false;
		//open image
		inputImage = imread("view3.png");
		//convert image from BGR to HSV colorspace
		cvtColor(inputImage, HSV, COLOR_BGR2HSV);
		//show the image
		imshow(windowName, inputImage);
		break;
	}
	
	//focus to this field
	this->Focus();
}




private: System::Void radioButton1_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
	destroyAllWindows();
	
	//set values for the HSV filter
	setValues(167, 54, 168, 256, 116, 226);
	
	//according to view apply structuring element
	switch (viewSelector) {
	case 0:
		p = 9;
		q = 9;
		r = 15;
		s = 18;
		break;
	case 1:
		p = 11;
		q = 9;
		r = 40;
		s = 30;
		break;
	case 2:
		p = 11;
		q = 9;
		r = 20;
		s = 35;
		break;
	}

	destroyAllWindows();
	mainProgram();
}

private: System::Void radioButton2_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
	destroyAllWindows();
	setValues(0, 26, 98, 51, 63, 126);
	switch (viewSelector) {
	case 0:
		p = 5;
		q = 6;
		r = 50;
		s = 50;
		break;
	case 1:
		p = 5;
		q = 6;
		r = 50;
		s = 50;
		break;
	case 2:
		p = 5;
		q = 6;
		r = 50;
		s = 50;
		break;
	}

	destroyAllWindows();
	mainProgram();
}

private: System::Void radioButton3_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
	destroyAllWindows();
	setValues(2, 107, 196, 4, 121, 221);
	switch (viewSelector) {
	case 0:
		p = 5;
		q = 5;
		r = 60;
		s = 60;
		break;
	case 1:
		p = 3;
		q = 3;
		r = 30;
		s = 30;
		break;
	case 2:
		p = 3;
		q = 5;
		r = 50;
		s = 39;
		break;
	}

	destroyAllWindows();
	mainProgram();
}

private: System::Void radioButton4_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
	destroyAllWindows();
	setValues(49, 0, 93, 70, 51, 158);
	switch (viewSelector) {
	case 0:
		p = 6;
		q = 5;
		r = 25;
		s = 80;
		break;
	case 1:
		p = 4;
		q = 3;
		r = 40;
		s = 80;
		break;
	case 2:
		p = 6;
		q = 4;
		r = 25;
		s = 40;
		break;
	}

	destroyAllWindows();
	mainProgram();
}

private: System::Void radioButton5_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
	destroyAllWindows();
	setValues(0, 0, 221, 0, 0, 235);
	switch (viewSelector) {
	case 0:
		p = 6;
		q = 4;
		r = 90;
		s = 60;
		break;
	case 1:
		p = 7;
		q = 3;
		r = 30;
		s = 80;
		break;
	case 2:
		p = 3;
		q = 6;
		r = 90;
		s = 90;
		break;
	}

	destroyAllWindows();
	mainProgram();
}

private: System::Void view01ToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
	//select the view
	comboBox1->SelectedIndex = 0;
}

private: System::Void view02ToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
	comboBox1->SelectedIndex = 1;
}

private: System::Void view03ToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
	comboBox1->SelectedIndex = 2;
}

private: System::Void athleticFieldToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
	//check whether input image is there or not
	if (inputImage.data) {
		radioButton1->Checked = true;
	}
	else {
		MessageBox::Show("Select a view first.", "Error", MessageBoxButtons::OK, MessageBoxIcon::Exclamation);
	}
}

private: System::Void innerGroundToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
	if (inputImage.data) {
		radioButton2->Checked = true;
	}
	else {
		MessageBox::Show("Select a view first.", "Error", MessageBoxButtons::OK, MessageBoxIcon::Exclamation);
	}
}

private: System::Void tennisCourtsToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
	if (inputImage.data) {
		radioButton3->Checked = true;
	}
	else {
		MessageBox::Show("Select a view first.", "Error", MessageBoxButtons::OK, MessageBoxIcon::Exclamation);
	}
}

private: System::Void mainGroundToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
	if (inputImage.data) {
		radioButton4->Checked = true;
	}
	else {
		MessageBox::Show("Select a view first.", "Error", MessageBoxButtons::OK, MessageBoxIcon::Exclamation);
	}
}

private: System::Void astroscienceLaboratoryToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
	if (inputImage.data) {
		radioButton5->Checked = true;
	}
	else {
		MessageBox::Show("Select a view first.", "Error", MessageBoxButtons::OK, MessageBoxIcon::Exclamation);
	}
}

private: System::Void exitToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
	//ask from user to confirm
	if (MessageBox::Show("Are you sure want to exit?", "Warning", MessageBoxButtons::YesNo, MessageBoxIcon::Warning) == Windows::Forms::DialogResult::Yes) {
		destroyAllWindows();
		Application::Exit();
	}
}

private: System::Void redToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
	//set values of the colors
	R = 255;
	G = 0;
	B = 0;
}

private: System::Void greenToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
	R = 0;
	G = 255;
	B = 0;
}

private: System::Void blueToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
	R = 0;
	G = 0;
	B = 255;
}

private: System::Void grayToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
	R = 255;
	G = 0;
	B = 255;
}

private: System::Void yellowToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
	R = 255;
	G = 255;
	B = 0;
}

private: System::Void toolStripMenuItem1_Click(System::Object^  sender, System::EventArgs^  e) {
	//check whether HSV matrix is assigned or not
	if (HSV.data) {
		cvtColor(inputImage, HSV, COLOR_BGR2HSV);
		imshow(windowName1, HSV);
	}
	else {
		MessageBox::Show("Please select a View and a Region", "Warning", MessageBoxButtons::OK, MessageBoxIcon::Exclamation);
	}
}

private: System::Void thresholdToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
	if (threshold.data) {
		imshow(windowName2, threshold);
	}
	else {
		MessageBox::Show("Please select a View and a Region", "Warning", MessageBoxButtons::OK, MessageBoxIcon::Exclamation);
	}
}

private: System::Void contoursToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
	if (threshold.data) {
		imshow(windowName3, drawing);
	}
	else {
		MessageBox::Show("Please select a View and a Region", "Warning", MessageBoxButtons::OK, MessageBoxIcon::Exclamation);
	}
}

private: System::Void helpToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
	//call the about page
	about^ page = gcnew about();
	page->ShowDialog();
}
};
}

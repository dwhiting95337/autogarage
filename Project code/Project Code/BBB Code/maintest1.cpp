// Main entry code OpenCV

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv/cvaux.h>
#include <opencv/ml.h>

#include <iostream>
#include <vector>

#include "DetectRegions.h"


using namespace std;
using namespace cv;



//This is for the take picture portion  It no longer detects motion because it does not need to.  That is the job
//of the sensor

//our sensitivity value to be used in the absdiff() function
//for higher sensitivity, use a lower value
const static int SENSITIVITY_VALUE = 40;
//size of blur used to smooth the intensity image output from absdiff() function
const static int BLUR_SIZE = 10;
//these two can be toggled by pressing 'd' or 't'
bool debugMode;
bool trackingEnabled;

bool found=false;
//end of global values for picture portion

bool take_picture();//Function for picture

string getFilename(string s) {

    char sep = '/';
    char sepExt='.';

    #ifdef _WIN32
        sep = '\\';
    #endif

    size_t i = s.rfind(sep, s.length( ));
    if (i != string::npos) {
        string fn= (s.substr(i+1, s.length( ) - i));
        size_t j = fn.rfind(sepExt, fn.length( ));
        if (i != string::npos) {
            return fn.substr(0,j);
        }else{
            return fn;
        }
    }else{
        return "";
    }
}

int main ( int argc, char** argv )
{
	cout<<"Facebook"<<endl;//For luck
    cout << "OpenCV Automatic Number Plate Recognition\n";


	while(!found)
	{
	//take_picture();

	//char* filename="C:\\Users\\Sam Vargas\\Desktop\\Senior Project\\shit.jpg";//This will grap photo from the camera
	char* filename = "testcar000.jpg"; 
   //char* filename="C:\\Users\\Sam Vargas\\Desktop\\Senior Project\\cars.jpg";//This line is for 
	//char* filename="C:\\Users\\Sam Vargas\\Desktop\\Senior Project\\test3.jpg";//This line is for 
	 //pictures on your comptuer 
	 
	 //char* filename = "test.jpg"; //IMPLEMENTED FOR BOARD USAGE	 
	
    Mat input_image;
	
    if(argc <= 2 )//this is for if you are putting in the image file manually.  ARGV represents each action taken and argc are total actions taken
    {
		
        
        input_image=imread(filename,1);//Supply Image to input_image
		
    }else{
        printf("Use:\n\t%s image\n",argv[0]);
        return 0;
    }        

    string filename_whithoutExt=getFilename(filename);//lets code get filename w/o ext
    cout << "working with file: "<< filename_whithoutExt << "\n";
    //Detect posibles plate regions
    DetectRegions detectRegions;    
    detectRegions.setFilename(filename_whithoutExt);
    detectRegions.saveRegions=false;
    detectRegions.showSteps=false;
    vector<Plate> posible_regions= detectRegions.run( input_image );    
    //SVM for each plate region to get valid car plates
    //Read file storage.
    FileStorage fs;
    //fs.open("C:\\Users\\Sam Vargas\\Desktop\\Senior Project\\Data.xml", FileStorage::READ);
	//fs.open("C:\\Users\\Sam Vargas\\Desktop\\Senior Project\\Senior Project\\Plates\\newdata.xml", FileStorage::READ);
	//fs.open("C:\\Users\\Sam Vargas\\Desktop\\Senior Project\\Senior Project\\Honda\\honda.xml", FileStorage::READ);
	
	fs.open("newdata.xml", FileStorage::READ); //IMPLEMENTED FOR BOARD USAGE
					if (!fs.isOpened())
						{
							cout<<"SVM.XML not being opened.  Fix this as soon as possible"<<endl;
							return -1;
						}
					
    Mat SVM_TrainingData;
    Mat SVM_Classes;
    fs["TrainingData"] >> SVM_TrainingData;
    fs["classes"] >> SVM_Classes;
    //Set SVM params
    CvSVMParams SVM_params;
    SVM_params.svm_type = CvSVM::C_SVC;
    SVM_params.kernel_type = CvSVM::LINEAR; //CvSVM::LINEAR;
    SVM_params.degree = 0;
    SVM_params.gamma = 1;
    SVM_params.coef0 = 0;
    SVM_params.C = 1;
    SVM_params.nu = 0;
    SVM_params.p = 0;
    SVM_params.term_crit = cvTermCriteria(CV_TERMCRIT_ITER, 1000, 0.01);
    //Train SVM
    CvSVM svmClassifier(SVM_TrainingData, SVM_Classes, Mat(), Mat(), SVM_params);

    //For each possible plate, classify with svm if it's a plate or no
    vector<Plate> plates;
	int correct_region=0;
    for(int i=0; i< posible_regions.size(); i++)
    {
		
        Mat img=posible_regions[i].plateImg;
        Mat p= img.reshape(1, 1);
        p.convertTo(p, CV_32FC1);
        int response = (int)svmClassifier.predict( p );
        if(response==1)
		{	
			//imread("````",1);
			imshow("img",img);

			
			//sometimes you will see me use Mat and others cv:: Mat they do the exact same thing
			//cv:: Mat original = cv::imread("C:\\Users\\Sam Vargas\\Desktop\\Senior Project\\check_size1.jpg"); //COMMENTED OUT BY ISRAEL
			cv:: Mat original = cv::imread("check_size.jpg"); 		
			
			cv::resize(img, img,original.size());
			imshow("image_2",img);
			
			//imwrite("C:\\Users\\Sam Vargas\\Desktop\\Senior Project\\check_size2.jpg",img); //COMMENTED OUT BY ISRAEL
			//imwrite("C:\\Users\\Sam Vargas\\Desktop\\Senior Project\\tiff.tif",img);	//COMMENTED OUT BY ISRAEL
			
			imwrite("check_size2.jpg", img); //FOR BOARD USAGE
			//imwrite("tiff.tif", img); //FOR BOARD USAGE
						
			correct_region=i;
			cout<<endl<<"Correct region i values is "<<correct_region<<endl;
			found=true;
            plates.push_back(posible_regions[i]);
		}
    }
		
    cout << "Num plates detected: " << plates.size() << "\n";

    //For each plate detected, recognize it with OCR
	//OCR ocr("C:\\Users\\Sam Vargas\\Desktop\\Senior Project\\Senior Project\\official\\please.xml");
	//OCR ocr("C:\\Users\\Sam Vargas\\Desktop\\Senior Project\\ORC.xml");
    //ocr.saveSegments=true;
    //ocr.DEBUG=false;
    //ocr.filename=filename_whithoutExt;
    for(int i=0; i< plates.size(); i++){
        Plate plate=plates[i];//C:\\Users\\Samuel Vargas\\Desktop\\Plates\\crop.jpg
				Mat cropimage;


			//cv:: Mat original = cv::imread("C:\\Users\\Sam Vargas\\Desktop\\Senior Project\\check_size1.jpg"); //COMMENTED OUT BY ISRAEL
			cv:: Mat original = cv::imread("check_size.jpg"); //SHOULD I BE CHANGING BACK TO CHECK_SIZE		
				
			
				//with california jpeg   The following lines are just for a check
				//cout<<"Y plate is: "<<plate.position.y<<" and height is: "<<plate.position.height<<endl;
				//cout<<"plate position y desired"<<plate.position.y+15<<" and height desired: "<<plate.position.height-20<<endl;
				
			//getting the plate image
				Rect roi(plate.position.x,plate.position.y, plate.position.width, plate.position.height);
				Mat image_roi = input_image(roi);
				imshow("image roi",image_roi); //NOT SURE WHAT THIS IMAGE IS
				image_roi.copyTo(cropimage);
				imshow ("Crop", cropimage);//this is the image to crop
				//imwrite("C:\\Users\\Sam Vargas\\Desktop\\Senior Project\\crop.jpg",image_roi);	//COMMENTED OUT BY ISRAEL
				//imwrite("C:\\Users\\Sam Vargas\\Desktop\\Senior Project\\notcrop.jpg",posible_regions[correct_region].plateImg);//COMMENTED OUT BY ISRAEL
				
				imwrite("crop.jpg", image_roi); //FOR BOARD USAGE
				imwrite("notcrop.jpg", posible_regions[correct_region].plateImg); //FOR BOARD USAGE
				
				
				//We are resizing the image.  For some reason code likes plates of the size 114 by 57.
				//this code below makes the image into that size.  Note the images we need are 800 by 600 but code 
				//does that for you
				//Mat needed=imread("C:\\Users\\Sam Vargas\\Desktop\\Senior Project\\crop.jpg");	//COMMENTED OUT BY ISRAEL
				
				Mat needed = imread("crop.jpg"); //FOR BOARD USAGE
								
				cv::resize(needed, needed, original.size());//actual resizing portion
				
				//Rect roi1(0,25, 135, 37);//x cord, y cord, width, height  This crops out the image
				Mat crops = cropimage;//cropimage(roi1);//puts image into Mat type
				//Mat crops = needed(roi1);//puts image into Mat type
				//imshow("needed",crops);//this was to show that the code was doing its things correctly

				//Need to make image grey
				Mat im_gray;
				cvtColor(crops,im_gray,CV_RGB2GRAY);
				Mat img_bw = im_gray>128;
				imshow("bw",img_bw);
				imshow("crops",crops);
				imwrite("plate.jpg",crops);
				//pass in grey image
				//imwrite("C:\\Users\\Sam Vargas\\Desktop\\Senior Project\\bw.jpg",img_bw); //COMMENTED OUT BY ISRAEL
				
				imwrite("bw.jpg", img_bw); //FOR BOARD USAGE
				
				plates[i].plateImg=img_bw;//This line only liked plates of the size 115 by 57
				//plates[i].plateImg=posible_regions[correct_region].plateImg;//This line allows all sizes to be accepted
				//It essentially just converts all plates to the right size.  For some reason the code doesn't like larger plates.
			plate=plates[i];
		imshow("plates[i]",plates[i].plateImg);


 //End of Tesseract part


        //string plateNumber=ocr.run(&plate);
        string licensePlate="Plate";
		//string licensePlate="Found";
        cout << "================================================\n";
        cout << "License plate number: "<< licensePlate << "\n";
        cout << "================================================\n";
        rectangle(input_image, plate.position, Scalar(0,0,200));
        putText(input_image, licensePlate, Point(plate.position.x, plate.position.y), CV_FONT_HERSHEY_SIMPLEX, 1, Scalar(0,0,200),2);
				
				
        if(false){
            imshow("Plate Detected seg", plate.plateImg); 
            cvWaitKey(0);
        }

    }
				
				

				imshow("Plate Detected", input_image);
	//the statement below is to keep the images up if plate is found
      if(found)
	  {
				for(;;)
       {
       int c;
       c = cvWaitKey(10);
       if( (char) c == 27)
       break;
				}
       }

	  char check='A';
	  cout<<"Do you want to break out of loop?(Y or y for yes): ";
	  cin>>check;
	  if (check=='Y'||check=='y')
		{
		imwrite("testplate.jpg", input_image); 
		cvWaitKey(5000); 		  
		break;
		}	  
		else{
		cvWaitKey(5000); 
		  check='A';
		}
	  
	   }
    return 0;
}


bool take_picture()
{
	cout<<"In take a picture function"<<endl;
		bool leave_while=false;
	Mat CARIMAGE;
	//Mat sizing=imread("C:\\Users\\Sam Vargas\\Desktop\\Senior Project\\Senior Project\\california.jpg"); //COMMENTED OUT BY ISRAEL
	
	Mat sizing = imread("california.jpg"); //FOR BOARD USAGE
	
	//set recording and startNewRecording initially to false.
	bool recording = false;
	bool startNewRecording = false;
	int inc=0;
	bool firstRun = true;
	//if motion is detected in the video feed, we will know to start recording.
	bool motionDetected = false;

	//pause and resume code (if needed)
	bool pause = false;
	//set debug mode and trackingenabled initially to false
	//these can be toggled using 'd' and 't'
	debugMode = false;
	trackingEnabled = true;
	//set up the matrices that we will need
	//the two frames we will be comparing
	Mat frame1,frame2;
	//their grayscale images (needed for absdiff() function)
	Mat grayImage1,grayImage2;
	//resulting difference image
	Mat differenceImage;
	//thresholded difference image (for use in findContours() function)
	Mat thresholdImage;
	//video capture object.
	VideoCapture capture;
	capture.open(0);
	VideoWriter oVideoWriter;//create videoWriter object, not initialized yet
	double dWidth = capture.get(CV_CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
	double dHeight = capture.get(CV_CAP_PROP_FRAME_HEIGHT); //get the height of frames of the video
	//set framesize for use with videoWriter
	Size frameSize(800, 600);

	if(!capture.isOpened()){
		cout<<"ERROR ACQUIRING VIDEO FEED\n";
		getchar();
		return -1;
	}
	while(1){


		//read first frame
		capture.read(frame1);
		//convert frame1 to gray scale for frame differencing
		cv::cvtColor(frame1,grayImage1,COLOR_BGR2GRAY);
		//copy second frame
		capture.read(frame2);
		//convert frame2 to gray scale for frame differencing
		cv::cvtColor(frame2,grayImage2,COLOR_BGR2GRAY);
		//perform frame differencing with the sequential images. This will output an "intensity image"
		//do not confuse this with a threshold image, we will need to perform thresholding afterwards.
		cv::absdiff(grayImage1,grayImage2,differenceImage);
		//threshold intensity image at a given sensitivity value
		cv::threshold(differenceImage,thresholdImage,SENSITIVITY_VALUE,255,THRESH_BINARY);
		if(debugMode==true){
			//show the difference image and threshold image
			cv::imshow("Difference Image",differenceImage);
			cv::imshow("Threshold Image", thresholdImage);
		}else{
			//if not in debug mode, destroy the windows so we don't see them anymore
			cv::destroyWindow("Difference Image");
			cv::destroyWindow("Threshold Image");
		}
		//blur the image to get rid of the noise. This will output an intensity image
		cv::blur(thresholdImage,thresholdImage,cv::Size(BLUR_SIZE,BLUR_SIZE));
		//threshold again to obtain binary image from blur output
		cv::threshold(thresholdImage,thresholdImage,SENSITIVITY_VALUE,255,THRESH_BINARY);
		
		


		



		
		//show our captured frame
		//imshow("Frame1",frame1);

		//check to see if a button has been pressed.
		//the 30ms delay is necessary for proper operation of this program
		//if removed, frames will not have enough time to referesh and a blank image will appear.
		//switch(waitKey(30)){

		cvWaitKey(5000); 		
		//case 114:
			//'r' has been pressed.
			//toggle recording mode
			recording =!recording;
			cvWaitKey(2000);//this is so the system will tell the code to record, and it will wait 2
			//allow for the car to get into the correct position.  This of course can be altered.
			capture.read(frame1);//this is here to ensure latest instance is grabbed for picture
			if (!recording)cout << "Recording Stopped" << endl;

			else cout << "Recording Started" << endl;

			capture>>CARIMAGE;
			resize(CARIMAGE,CARIMAGE,sizing.size());
			//imwrite("C:\\Users\\Sam Vargas\\Desktop\\Senior Project\\shit.png",CARIMAGE);//COMMENTED OUT BY ISRAEL
			//imwrite("C:\\Users\\Sam Vargas\\Desktop\\Senior Project\\shit.jpg",CARIMAGE);//COMMENTED OUT BY ISRAEL
			
			imwrite("shit.jpg", CARIMAGE); //FOR BOARD USAGE
	
			leave_while=true;
			break;

		

		//}
		if (leave_while)
			break;

	}
	cout<<"out of switch statement"<<endl;

	
}

// BSD 2-Clause License
//
// Copyright (c) 2018, Jörn Rath
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, this
//    list of conditions and the following disclaimer.

// 2. Redistributions in binary form must reproduce the above copyright notice,
//      this list of conditions and the following disclaimer in the documentation
//      and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef CAMERACAPTURE_H_
#define CAMERACAPTURE_H_

#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include "opencv2/opencv.hpp"

//////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Simple wrapper around OpenCV's VideoCapture class for reading the Raspberry Pi camera               //
/// Retrieves images from the camera in a separate thread                                               //
/// Allows to get the newest camera image even if the read() method is not called at camera frame rate  //
///                                                                                                     //
/// https://github.com/JoernR/CameraCapture                                                             //
//////////////////////////////////////////////////////////////////////////////////////////////////////////

class CameraCapture {
public:
	/// Constructor
	/// Initialized the wrapper and opens the the OpenCV capture device
	/// @param cam The video capture device id
	CameraCapture(int cam);
	
	/// Destructor
	virtual ~CameraCapture();

	/// Copies the last image retrieved from the camera
	/// @param Image Destination for the image
	/// @return True if valid image was copied 
	bool read(cv::OutputArray image);
	
	/// Check if the camera is opened
	/// @return True if succeeded
	bool isOpened();
	
	/// Sets the capture device properties
	/// Same arguments as the OpenCV VideoCapture::set method
	/// @return True if succeeded
	bool set(int propId, double value);

private:
	/// Gets the newest image from the camera in the background
	void run();

	/// VideoCapture object used for reading the camera
	cv::VideoCapture m_cap;

	/// read status
	bool m_readRetVal;
	
	/// Internal storage for the latest image
	cv::Mat m_image;

	/// Thread that runs the run() method
	std::thread m_runThread;
	
	/// Mutex for handling access to m_image member
	std::mutex m_readMutex;

};

#endif /* CAMERACAPTURE_H_ */

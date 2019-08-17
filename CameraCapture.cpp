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

#include "CameraCapture.h"

CameraCapture::CameraCapture(int cam) : m_cap(cam){
	m_readRetVal = false;
	m_runThread = std::thread(&CameraCapture::run, this);
	m_runThread.detach();
}

CameraCapture::~CameraCapture() {

}

bool CameraCapture::read(cv::OutputArray image){
	//Lock the readMutex and wait if not available. Gets released when leaving the method
	std::lock_guard<std::mutex> lock(m_readMutex);

	//Copy the last read image
	m_image.copyTo(image);
	return m_readRetVal;
}

bool CameraCapture::isOpened(){
	return m_cap.isOpened();
}

bool CameraCapture::set(int propId, double value){
	return m_cap.set(propId, value);
}

void CameraCapture::run(){
	// Make sure the camera was opened
	if(!m_cap.isOpened()){
		std::cout << "CameraCapture: Could not open camera!" << std::endl;
		return;
	}
	
	while(1){
		//Request new image. The grab() method blocks until image is available
		if(!m_cap.grab()){
			m_readRetVal = false;
			std::this_thread::sleep_for (std::chrono::milliseconds(100));
			std::cout << "CameraCapture: Error while grabbing new image!" << std::endl;
			continue;
		}

		//Lock the read mutex and wait if not available
		std::unique_lock<std::mutex> lock(m_readMutex);

		//Deode and save the last image
		m_readRetVal = m_cap.retrieve(m_image);

		//Unlock the read mutex: Allows reading the new image with read() method
		lock.unlock();
	}
}

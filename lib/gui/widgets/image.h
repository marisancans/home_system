#include <opencv2/opencv.hpp>
#include <GL/glew.h>
#include "imgui/imgui.h"

struct ImageWidget {
   cv::Mat img;
   GLuint textureID{0};

   void SLOT_set_image(cv::Mat& mat)
   {
      img = std::move(mat);
      bindCurrImg();
   }

   void draw()
   {
      SLOT_set_image(img);

      ImGui::Begin("Image", NULL);
      
      ImVec2 matSize(float(img.cols), float(img.rows));

      if(textureID)
         ImGui::Image((void*)(intptr_t)textureID, matSize);

      ImGui::End();
   }

   void bindCurrImg(){
      if(textureID)
         glDeleteTextures(1, &textureID);
    
      bindCVMat2GLTexture();
   }

   void bindCVMat2GLTexture()
   {
      if(img.empty())
         return 

      //glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
      glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
      glGenTextures(1, &textureID);
      glBindTexture(GL_TEXTURE_2D, textureID);
      
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      
      // Set texture clamping method
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

      auto rowLen = size_t(img.step) / img.elemSize();
      glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
      glPixelStorei(GL_UNPACK_ROW_LENGTH, rowLen);
      
      glTexImage2D(GL_TEXTURE_2D,         // Type of texture
                  0,                   // Pyramid level (for mip-mapping) - 0 is the top level
                  GL_RGB,              // Internal colour format to convert to
                  img.cols,          // Image width  i.e. 640 for Kinect in standard mode
                  img.rows,          // Image height i.e. 480 for Kinect in standard mode
                  0,                   // Border width in pixels (can either be 1 or 0)
                  GL_BGR,              // Input image format (i.e. GL_RGB, GL_RGBA, GL_BGR etc.)
                  GL_UNSIGNED_BYTE,    // Image data type
                  img.ptr());        // The actual image data itself
   }
};
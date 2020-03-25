
#pragma once

#include "imgui/imgui.h"
#include <mutex>
#include <string.h>

struct LogWidget
{
  enum Level{ kDebug = 0, kInfo, kWarn, kError };
  
  ImGuiTextBuffer buf;
  ImGuiTextFilter filter;
  bool scrollToBottom;
  ImVector<int> lineOffsets; 
  
  void clear()
  { 
    buf.clear();
    lineOffsets.clear();
  }

  void SLOT_log(std::string msg){
    msg += "\n";
    buf.appendf("%s", msg.c_str());
    scrollToBottom = true;

    // find seperator and remove line
    if(buf.size() > 20000){
      
      for(int i=0; i<buf.Buf.size(); i++){
        if(buf.Buf[i] == '\n'){
          buf.Buf.erase(buf.Buf.begin(), std::next(buf.Buf.begin(), i+1));
          break;
        }
      }
    }
  }

  void draw(){
    ImGui::SetNextWindowPos({ 0,0 });
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);

    // ImGui::SetNextWindowSize(ImVec2(500,400));
    ImGui::Begin("Log", NULL);
    if (ImGui::Button("Clear")) clear();
    ImGui::SameLine();
    bool copy = ImGui::Button("Copy");
    ImGui::SameLine();
    filter.Draw("Filter", -100.0f);
    ImGui::Separator();
    ImGui::BeginChild("scrolling");
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0,1));
    if(copy) ImGui::LogToClipboard();

    if(filter.IsActive()){
      const char* buf_begin = buf.begin();
      const char* line = buf_begin;

      for(int line_no = 0; line != NULL; line_no++){
          const char* line_end = (line_no < lineOffsets.Size) ? buf_begin + lineOffsets[line_no] : NULL;
          
          if(filter.PassFilter(line, line_end))
            ImGui::TextUnformatted(line, line_end);

          line = line_end && line_end[1] ? line_end + 1 : NULL;
      }
    } else {
      ImGui::TextUnformatted(buf.begin());
    }

    if(scrollToBottom)
      ImGui::SetScrollHere(1.0f);
    scrollToBottom = false;

    ImGui::PopStyleVar();
    ImGui::EndChild();
    ImGui::End();
    ImGui::PopStyleVar(2);
  }
};

#include <GL/glew.h>
#include <GLFW//glfw3.h>
#include <cstdio>
#include <string>
#include <fstream>
#include <vector>
#include <MediaReader.h>
#include <VideoFrame.h>
#include <AudioFrame.h>
#include <MediaFrame.h>
#include <Misc.h>
#include <ALContext.h>
#include <ALSource.h>
#include <GLContext.h>
#include <GLProgram.h>
#include <thread>
#include <queue>
#include "default_frag.h"
#include "default_vert.h"

static const GLfloat g_vertex_buffer_data[] = {
         -1.0f, -1.0f, 0.0f,
          1.0f, -1.0f, 0.0f,
          1.0f,  1.0f, 0.0f,
          1.0f,  1.0f, 0.0f,
         -1.0f,  1.0f, 0.0f,
         -1.0f, -1.0f, 0.0f,
};

static const GLfloat g_uv_buffer_data[] = {
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 1.0f
};

int main(void)
{
  gul::ALContext* contextAL  = new gul::ALContext();
  contextAL->Initialize();
  gul::ALSource* source = new gul::ALSource();
  source->Initialize();

  gul::GLContext contextGL;
  contextGL.Initialize(1024, 768, "gulVideoPlayer");

  gul::GLProgram program;
  program.Initialize();
  program.CompileShader(GL_VERTEX_SHADER, default_vert.data);
  program.CompileShader(GL_FRAGMENT_SHADER, default_frag.data);
  program.Link();
  program.Use();

  // texture sampler is texture unit 0
  GLuint sampler = glGetUniformLocation(program.GetGLId(), "sampler");
  glUniform1i(sampler, 0);

  // Create one OpenGL texture
  GLuint tex;
  glGenTextures(1, &tex);

  // quad
  GLuint vertexbuffer;
  glGenBuffers(1, &vertexbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  glVertexAttribPointer(
          0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
          3,                  // size
          GL_FLOAT,           // type
          GL_FALSE,           // normalized?
          0,                  // stride
          (void*)0            // array buffer offset
  );

  // texture coordinates
  GLuint uvbuffer;
  glGenBuffers(1, &uvbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data), g_uv_buffer_data, GL_STATIC_DRAW);

  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
  glVertexAttribPointer(
          1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
          2,                                // size : U+V => 2
          GL_FLOAT,                         // type
          GL_FALSE,                         // normalized?
          0,                                // stride
          (void*)0                          // array buffer offset
  );


  // video reader
  gul::MediaReader loader(gul::String("/home/pfeuti/Code/gul/test/data/video/firefly_long.mkv"));
  loader.Open();
  gul::MediaFrame frames[25];
  std::queue<gul::MediaFrame*> freeFrame;
  for(int i = 0; i < 25; ++i)
    freeFrame.push(&frames[i]);
  std::queue<gul::MediaFrame*> usedFrame;

  long duration;
  do
  {
    if(!freeFrame.empty())
    {
      gul::MediaFrame* f = freeFrame.front();
      freeFrame.pop();
      loader.GetNext(*f);
      usedFrame.push(f);
      if(f->HasVideoFrame())
      {
          duration = (f->GetVideoFrame().GetPresentationTime() - glfwGetTime())*1000;
          if(duration > 0)
          {
            std::chrono::milliseconds dura( duration );
            std::this_thread::sleep_for( dura );
          }

        glClear( GL_COLOR_BUFFER_BIT );

        //glBindTexture(GL_TEXTURE_2D, f->HasVideoFrame()->GetGLTexture());
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//        glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, f->GetVideoFrame().GetWidth(), f->GetVideoFrame().GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, f->GetVideoFrame().GetDataConst());

        // Draw the triangle !
        glDrawArrays(GL_TRIANGLES, 0, 6); // 3 indices starting at 0 -> 1 triangle

        contextGL.SwapBuffers();
      }
      else if(f->HasAudioFrame())
      {
        source->AddBuffer(f->GetAudioFrame().GetALBuffer());
        source->Play();
      }
    }
    else
    {
      std::chrono::milliseconds dura( duration );
      std::this_thread::sleep_for( dura );
    }

    source->RemoveBuffers();
    for(size_t i = 0; i < usedFrame.size(); ++i)
    {
      gul::MediaFrame* uf = usedFrame.front();
      if(uf->HasVideoFrame() || (uf->HasAudioFrame() && source->IsBufferPlayed(uf->GetAudioFrame().GetALBuffer())))
      {
        usedFrame.pop();;
        freeFrame.push(uf);
      }
    }

  } while(loader.IsFrameValid());

  loader.Close();

  // Cleanup VBO
  glDeleteBuffers(1, &vertexbuffer);
  glDeleteBuffers(1, &uvbuffer);
  glDeleteTextures(1, &tex);

  delete source;
  delete contextAL;

  return 0;
}

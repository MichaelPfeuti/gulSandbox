#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>
#include <GL/glew.h>
#include <GL/glfw.h>
#include <cstdio>
#include <string>
#include <fstream>
#include <vector>
#include <MediaReader.h>
#include <VideoFrame.h>
#include <AudioFrame.h>
#include <MediaFrame.h>

GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path)
{

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if(VertexShaderStream.is_open()){
		std::string Line = "";
		while(getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}else{
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if(FragmentShaderStream.is_open()){
		std::string Line = "";
		while(getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}



	GLint Result = GL_FALSE;
	int InfoLogLength;



	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}



	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}



	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}








#define CHECK_ERROR() \
  if ((error = alGetError()) != AL_NO_ERROR) { \
    fprintf(stderr, "\n%d\n\n", error); \
    return -1; \
  }




int main(void)
{
  int error;
  ALCdevice* device = alcOpenDevice(NULL);
  ALCcontext* context = alcCreateContext(device,NULL);
  alcMakeContextCurrent(context);
  alGetError();
  ALuint source;
  alGenSources(1,&source);
  CHECK_ERROR();



  // Initialise GLFW
  if( !glfwInit() )
  {
          fprintf( stderr, "Failed to initialize GLFW\n" );
          return -1;
  }

  glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
  glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 3);
  glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Open a window and create its OpenGL context
  if( !glfwOpenWindow( 1024, 768, 0,0,0,0, 32,0, GLFW_WINDOW ) )
  {
          fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
          glfwTerminate();
          return -1;
  }

  // Initialize GLEW
  glewExperimental = true; // Needed for core profile
  if (glewInit() != GLEW_OK) {
          fprintf(stderr, "Failed to initialize GLEW\n");
          return -1;
  }

  glfwSetWindowTitle( "Tutorial 02" );

  // Ensure we can capture the escape key being pressed below
  glfwEnable( GLFW_STICKY_KEYS );

  // Dark blue background
  glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

  GLuint VertexArrayID;
  glGenVertexArrays(1, &VertexArrayID);
  glBindVertexArray(VertexArrayID);

  // Create and compile our GLSL program from the shaders
  GLuint programID = LoadShaders( "default.vert", "default.frag" );


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

  // Create one OpenGL texture
  GLuint tex;
  glGenTextures(1, &tex);


  GLuint TextureID  = glGetUniformLocation(programID, "myTextureSampler");

  GLuint vertexbuffer;
  glGenBuffers(1, &vertexbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

  GLuint uvbuffer;
  glGenBuffers(1, &uvbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data), g_uv_buffer_data, GL_STATIC_DRAW);

  gul::MediaReader loader(gul::String("/home/pfeuti/Code/gul/test/data/video/firefly_long.mov"));
  loader.Open();
  gul::MediaFrame frame;

  int i = 0;
  do
  {

      loader.GetNext(frame);
      if(frame.HasVideoFrame())
      {
          float duration = frame.GetVideoFrame().GetPresentationTime() - glfwGetTime();
          if(duration > 0)

            glfwSleep(duration);
        glClear( GL_COLOR_BUFFER_BIT );

        glUseProgram(programID);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, frame.GetVideoFrame().GetWidth(), frame.GetVideoFrame().GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, frame.GetVideoFrame().GetDataConst());

        glUniform1i(TextureID, 0);

        // 1rst attribute buffer : vertices
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

        // Draw the triangle !
        glDrawArrays(GL_TRIANGLES, 0, 6); // 3 indices starting at 0 -> 1 triangle

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

        // Swap buffers
        glfwSwapBuffers();
      }
      else if(frame.HasAudioFrame())
      {
        float duration = frame.GetVideoFrame().GetPresentationTime() - glfwGetTime();
        if(duration > 0)
          glfwSleep(duration);

        ALuint buffer;
        alGenBuffers(1, &buffer);
        CHECK_ERROR();
        alBufferData(buffer,
                     AL_FORMAT_STEREO16,
                     frame.GetAudioFrame().GetData(),
                     frame.GetAudioFrame().GetDataSize(),
                     frame.GetAudioFrame().GetSampleRate());
        CHECK_ERROR();
        alSourceQueueBuffers(source, 1, &buffer);
        CHECK_ERROR();
        int state;
        alGetSourcei(source, AL_SOURCE_STATE, &state);
        if(state != AL_PLAYING)
        {
          alSourcePlay(source);
          CHECK_ERROR();
        }
      }

      ALint processed;
      ALuint buf;
      alGetSourcei(source, AL_BUFFERS_PROCESSED, &processed);
      while (processed--) {
          alGetError();
          alSourceUnqueueBuffers(source, 1, &buf);
      }



  } while( glfwGetKey( GLFW_KEY_ESC ) != GLFW_PRESS &&
           glfwGetWindowParam( GLFW_OPENED )  &&
           loader.IsFrameValid());

  loader.Close();
  glfwTerminate();

  // Cleanup VBO
  glDeleteBuffers(1, &vertexbuffer);
  glDeleteBuffers(1, &uvbuffer);
  glDeleteTextures(1, &tex);
  glDeleteVertexArrays(1, &VertexArrayID);

  alSourceStop(source);
  CHECK_ERROR();
  alDeleteSources(1, &source);
  CHECK_ERROR();
  //alDeleteBuffers(1, &buffer);
  //CHECK_ERROR();
  alcDestroyContext(context);
  CHECK_ERROR();
  alcCloseDevice(device);
  CHECK_ERROR();

  return 0;
}

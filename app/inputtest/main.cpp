#include <zeitgeist/zeitgeist.h>
#include <kerosin/kerosin.h>
#include <SDL/SDL.h>
#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/gl.h>

using namespace boost;
using namespace kerosin;
using namespace std;
using namespace zeitgeist;

shared_ptr<LogServer> gLog;
bool		gWantsToQuit = false;

void initSDL()
{
	if( SDL_Init(SDL_INIT_VIDEO) < 0 )
	{
        gLog->Error().Printf("Couldn't initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }
	atexit(SDL_Quit);

	const SDL_VideoInfo* info = SDL_GetVideoInfo();

	SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 24 );
    SDL_GL_SetAttribute( SDL_GL_STENCIL_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

	int flags = SDL_OPENGL;
	//int flags = SDL_OPENGL| SDL_FULLSCREEN;
	
	SDL_Surface *screen = SDL_SetVideoMode(640, 480, info->vfmt->BitsPerPixel, flags);
	if ( screen == NULL )
	{
		gLog->Error().Printf("Couldn't set 640x480 video mode: %s\n", SDL_GetError());
		exit(1);
    }
}

static void processEvents( void )
{
    // Our SDL event placeholder.
    SDL_Event event;

    // Grab all the events off the queue.
    while( SDL_PollEvent( &event ) )
	{
        switch( event.type )
		{
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE)
			{
				SDL_Event *event = new SDL_Event;
				event->type = SDL_QUIT;
				SDL_PushEvent(event);				
			}
			break;
        case SDL_QUIT:
            // Handle quit requests (like Ctrl-c).
            gWantsToQuit = true;
            break;
        }
    }
}

int main(int argc, char **argv)
{
	Zeitgeist	zg;

	shared_ptr<CoreContext> context = zg.CreateContext();
	Kerosin					kCore(zg);

	gLog = shared_static_cast<LogServer>(context->Get("/sys/server/log"));

	initSDL();

	shared_ptr<ScriptServer> scriptServer = shared_static_cast<ScriptServer>(context->New("zeitgeist/ScriptServer", "/sys/server/script"));

	scriptServer->Init();
	scriptServer->Run("inputtest.rb");

	shared_ptr<InputServer> inputServer = shared_static_cast<InputServer>(context->Get("/sys/server/input"));

	unsigned int prevTime = SDL_GetTicks();
	int frames = 0;

	inputServer->BindCommand("^", 1);
	inputServer->BindCommand("ß", 1);
	inputServer->BindCommand("<", 1);
	inputServer->BindCommand("a", 1);
	inputServer->BindCommand("shift abc", 1);

	while(!gWantsToQuit)
	{
		frames ++;
        // Process incoming events.
		InputServer::Input input;
		while (inputServer->GetInput(input))
		{
			//gLog->Normal() << "Input: " << (int)input.type << " '" << (char)input.code << "' '" << ch << "' " << (int)input.data.l << endl;
			if (input.type == InputServer::eButton && input.data.l)
			{
				gLog->Normal().Printf("%d - ", input.code);
				char ch;
				if (inputServer->TranslateCode(input.code, 0, ch) && input.data.l)
					gLog->Normal().Printf("%c\n", ch);
				else
					gLog->Normal().Printf("\n");
			}
		}

        processEvents();
		glClearColor(0,0,0,0);
		glClear(GL_COLOR_BUFFER_BIT);
		SDL_GL_SwapBuffers();
    }

	inputServer->BindCommand("shift +0", 1);
	inputServer->BindCommand("shift +a", 1);
	inputServer->BindCommand("+a", 1);
	inputServer->BindCommand("a", 1);
	inputServer->BindCommand("shift abc", 1);
	unsigned int time = SDL_GetTicks() - prevTime;

	gLog->Normal() << "Average FPS: " << 1000.0f*frames / (float)time << endl;

	return 0;
}

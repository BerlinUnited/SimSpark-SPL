#include <zeitgeist/zeitgeist.h>
#include <kerosin/kerosin.h>
#ifndef WIN32
#include <SDL/SDL.h>
#else
#include <include/SDL.h>
#endif 
#ifdef _WIN32
#include <windows.h>
#endif
#if HAVE_CONFIG_H
#include <sparkconfig.h>
#endif

using namespace boost;
using namespace kerosin;
using namespace zeitgeist;

bool            gWantsToQuit = false;

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
        Zeitgeist       zg("." PACKAGE_NAME);

        shared_ptr<CoreContext> context = zg.CreateContext();
        Kerosin                                 kCore(zg);

        shared_ptr<ScriptServer> scriptServer = shared_static_cast<ScriptServer>(context->Get("/sys/server/script"));
        scriptServer->Run("script/init.rb");

        scriptServer->Run("script/fonttest.rb");

        shared_ptr<FontServer> fontServer = shared_static_cast<FontServer>(context->Get("/sys/server/font"));
        if (fontServer.get() == NULL) return 1;

        shared_ptr<kerosin::Font> font = fontServer->GetFont("fonts/VeraMono.ttf", 24);
        if (font.get() == NULL) return 1;

        shared_ptr<ImageServer> imageServer = shared_static_cast<ImageServer>(context->New("kerosin/ImageServer", "/sys/server/image"));
        shared_ptr<TextureServer> textureServer = shared_static_cast<TextureServer>(context->New("kerosin/TextureServer", "/sys/server/texture"));

        imageServer->Load("data/image/jpg.jpg");
        /* imageServer->Save(image, "jpg.tga");
        image = imageServer->Load("image/bmp.bmp");
        imageServer->Save(image, "bmp.tga"); */
        //imageServer->Load("image/png.png");
/*
        imageServer->Save(image, "png.tga");
        image = imageServer->Load("image/png-gray.png");
        imageServer->Save(image, "png-gray.tga");
        image = imageServer->Load("image/tga-gray.tga");
        imageServer->Save(image, "tga-gray.tga");*/
        shared_ptr<Texture> tex = textureServer->GetTexture("data/image/jpg.jpg");

        while(!gWantsToQuit)
        {
        // Process incoming events.
                Input input;

                processEvents();
                glClearColor(0,0,0,0);
                glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
                glColor3f(1,1,1);
                tex->Bind();
                tex->Enable();
                glBegin(GL_TRIANGLES);
                        glTexCoord2f(0,0);
                        glVertex2f(0,0);
                        glTexCoord2f(255,0);
                        glVertex2f(255,0);
                        glTexCoord2f(255,255);
                        glVertex2f(255,255);
                glEnd();
                tex->Disable();

                fontServer->Begin();
                font->Bind();
                glColor3f(1,0,0);
                font->DrawString(0, 100, "This is a simple Test, 1!@0~*?^\\/\"");
                //font->Dump();
                fontServer->End();
                SDL_GL_SwapBuffers();
        }

        return 0;
}

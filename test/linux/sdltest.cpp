
#include <SDL2/SDL.h>
#include <iostream>

using namespace std;

int main () {

    if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0) {
        cout<<"Unable to init SDL: " << SDL_GetError()<<endl;
        return 1;
    }

    int sdldrinumb = SDL_GetNumVideoDrivers();
    cout<<"sdl video driver number: " << sdldrinumb<<endl;

    for (int i = 0; i < sdldrinumb; i++) {
       cout<<"sdl video driver " << i << " name: "<<  SDL_GetVideoDriver(i) << endl;
    }
    
    int sdlRender = SDL_GetNumRenderDrivers();
    cout<<"sdl render driver numer: " << sdldrinumb<<endl;
    for (int i = 0; i < sdldrinumb; i++) {
        SDL_RendererInfo ri;
        int ret =  SDL_GetRenderDriverInfo(i, &ri);
        if (ret == -1) {
            cout<<"get sdl render info failed:  " << i << ": "<<  ret << endl;
        } else {
            cout<<"sdl render  " << i << " name: "<<  ri.name << ", flag:"<< ri.flags <<endl;
        }
    }


    int sdlAudioDriverNumber = SDL_GetNumAudioDrivers();
    cout<<"sdl audio driver number: " << sdlAudioDriverNumber<<endl;
    for (int i = 0; i < sdlAudioDriverNumber; i++) {
       cout<<"sdl audio driver " << i << " name: "<<  SDL_GetAudioDriver(i) << endl;
    }

    int audioPlayDeviceNumber = SDL_GetNumAudioDevices(0);
    cout<<"sdl audio play device number: " << audioPlayDeviceNumber<<endl;
    for (int i = 0; i < audioPlayDeviceNumber; i++) {
       cout<<"sdl audio play " << i << " name: "<<  SDL_GetAudioDeviceName(i, 0) << endl;
    }

    int audioCaptureDeviceNumber = SDL_GetNumAudioDevices(1);
    cout<<"sdl audio capture device number: " << audioCaptureDeviceNumber<<endl;
    for (int i = 0; i < audioCaptureDeviceNumber; i++) {
       cout<<"sdl audio capture " << i << " name: "<<  SDL_GetAudioDeviceName(i, 1) << endl;
    }

    cout<<"end "<<endl;
}



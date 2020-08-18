#include "Includes.hpp"

#define MOVEMENT_SPEED 0.08

SDL_Window *window;
SDL_Event event;
SDL_GLContext glContext;

const Uint8 *key=SDL_GetKeyboardState(NULL);

struct WindowData{
    int WIDTH=800, HEIGHT=800;
    int POS_X=SDL_WINDOWPOS_CENTERED, POS_Y=SDL_WINDOWPOS_CENTERED;
    const char *TITLE="TEST";
    int FLAG=SDL_WINDOW_OPENGL;
} WindowData;

struct SquareData{
    int WIDTH=100, HEIGHT=100;
    float POS_X=0, POS_Y=0;
    SDL_Color FILLCOLOR={255, 0, 0};
} SquareData;

bool isOpen(){
    return window != NULL ? true : false;
}

void initGL(){
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

    glContext=SDL_GL_CreateContext(window);
    
    SDL_GL_SetSwapInterval(SDL_FALSE);
}

void pollEvent(){
    while(SDL_PollEvent(&event)){
        ImGui_ImplSDL2_ProcessEvent(&event);
        switch(event.type){
            case SDL_QUIT:
                SDL_DestroyWindow(window);
                window=NULL;
                break;
        }
    }
}

void clear(){
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    glViewport(0, 0, WindowData.WIDTH, WindowData.HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, WindowData.WIDTH, WindowData.HEIGHT, 0, -10, 10);
}

void display(){
    ImGui::Render();
    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(window);
}

void initImGui(){
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForOpenGL(window, glContext);
    ImGui_ImplOpenGL2_Init();
    ImGuiIO &io=ImGui::GetIO(); (void)io;
}

char width[100]={'1', '0', '0'};
char height[100]={'1', '0', '0'};
char x[100]={'0'};
char y[100]={'0'};

int getWidth(){
    string w;
    for(int i=0;i<sizeof(width);i++)
        w+=width[i];
    return atoi(w.c_str());
}

int getHeight(){
    string h;
    for(int i=0;i<sizeof(height);i++)
        h+=height[i];
    return atoi(h.c_str());
}

int getX(){
    string _x;
    for(int i=0;i<sizeof(x);i++)
        _x+=x[i];
    return atoi(_x.c_str());
}

int getY(){
    string _y;
    for(int i=0;i<sizeof(y);i++)
        _y+=y[i];
    return atoi(_y.c_str());
}

float r=(float)66/255, g=(float)135/255, b=(float)245/255, a=1;

void updateWindowData(){
    SDL_GetWindowSize(window, &WindowData.WIDTH, &WindowData.HEIGHT);
    SDL_GetWindowPosition(window, &WindowData.POS_X, &WindowData.POS_Y);
}

bool controller=false;

void updateCharacterPosition(){
    for(int i=0;i<IM_ARRAYSIZE(x);i++)
        x[i]=0;
    for(int i=0;i<IM_ARRAYSIZE(y);i++)
        y[i]=0;
    for(int i=0;i<to_string(SquareData.POS_X).size();i++)
        x[i]=to_string(SquareData.POS_X)[i];
    for(int i=0;i<to_string(SquareData.POS_Y).size();i++)
        y[i]=to_string(SquareData.POS_Y)[i];
}

void characterController(){
    if(key[SDL_SCANCODE_D]){
        if(SquareData.POS_X<=WindowData.WIDTH-SquareData.WIDTH){
            SquareData.POS_X+=MOVEMENT_SPEED;
            updateCharacterPosition();
        }
    }
    if(key[SDL_SCANCODE_A]){
        if(SquareData.POS_X>=0){
            SquareData.POS_X-=MOVEMENT_SPEED;
            updateCharacterPosition();
        }
    }
    if(key[SDL_SCANCODE_W]){
        if(SquareData.POS_Y>=0){
            SquareData.POS_Y-=MOVEMENT_SPEED;
            updateCharacterPosition();
        }
    }
    if(key[SDL_SCANCODE_S]){
        if(SquareData.POS_Y<=WindowData.HEIGHT-SquareData.HEIGHT){
            SquareData.POS_Y+=MOVEMENT_SPEED;
            updateCharacterPosition();
        }
    }
}

string controllerSting="Enable Controller";

int main(){
    window=SDL_CreateWindow(WindowData.TITLE, WindowData.POS_X, 
                            WindowData.POS_Y, WindowData.WIDTH, 
                            WindowData.HEIGHT, WindowData.FLAG);

    SDL_SetWindowResizable(window, SDL_TRUE);

    initGL();

    initImGui();

    bool ddos=false;

    while(window!=NULL){
        pollEvent();

        updateWindowData();
        clear();

        ImGui_ImplOpenGL2_NewFrame();
        ImGui_ImplSDL2_NewFrame(window);
        ImGui::NewFrame();

        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(r, g, b, a));
        ImGui::Begin("Square Data");
        ImGui::SetWindowPos(ImVec2(WindowData.WIDTH-200, 0));
        ImGui::PopStyleColor();
        ImGui::SetWindowSize(ImVec2(200, 190));
        ImGui::InputText("Width", width, IM_ARRAYSIZE(width));
        ImGui::InputText("Height", height, IM_ARRAYSIZE(height));
        ImGui::InputText("X", x, IM_ARRAYSIZE(x));
        ImGui::InputText("Y", y, IM_ARRAYSIZE(y));
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 1));
        if(ImGui::Button("Change Data", ImVec2(130, 25))){
            SquareData.WIDTH=getWidth();
            SquareData.HEIGHT=getHeight();
            SquareData.POS_X=getX();
            SquareData.POS_Y=getY();
        }
        while(ImGui::Button(controllerSting.c_str(), ImVec2(130, 25))){
            if(controller){
                controller=false;
                controllerSting="Enable Controller";
                break;
            }
            else{
                controller=true;
                controllerSting="Disable Controller";
                break;
            }
        }
        ImGui::PopStyleColor();
        ImGui::End();

        if(controller)
            characterController();

        glPushMatrix();
        glTranslatef(SquareData.POS_X, SquareData.POS_Y, 0);
        glBegin(GL_POLYGON);
            glColor3ub(SquareData.FILLCOLOR.r, 
            SquareData.FILLCOLOR.g, SquareData.FILLCOLOR.b);
            glVertex2f(0,                                0);
            glVertex2f(SquareData.WIDTH,                 0);
            glVertex2f(SquareData.WIDTH, SquareData.HEIGHT);
            glVertex2f(0,                SquareData.HEIGHT);
        glEnd();
        glPopMatrix();

        display();
    }
}
#include <iostream>
#include "Render/render.h"
#include "Physics/steviePhysics.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"

class object;

render* stevieRender = new render();
steviePhysics* physics = new steviePhysics();

std::vector<object*> squares;

vector startPosition(0, -2);

float velocity = 5.0f;

float k = 1.0;

enum demos { PROJECTILE, PENDULUM };

demos currentDemo = PROJECTILE;

class object {
    renderSquare* m_prSquare;
    physicsSquare* m_ppSquare;
public:
    object(renderSquare* rSquare, physicsSquare* pSquare){
        m_prSquare = rSquare;
        m_ppSquare = pSquare;
    }

    void update() {
        //sync physic entity and render entity
        m_prSquare->setPosition(m_ppSquare->getPosition().x, m_ppSquare->getPosition().y);
    }

};

void initScene() {
   
 
}

void initImGui() {

    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(stevieRender->window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

}
void spawnCube(vector dir) {


    renderSquare* rsquare = new renderSquare(0.1, glm::vec3(1.0f, 1.0, 0.0));
    stevieRender->add(rsquare);
 
    vector x0 = startPosition;
   
    vector v0 = dir * velocity;
    double mass = 1.0;

    physicsSquare* psquare = new physicsSquare(x0, v0, mass, k);
    physics->add(psquare);
   
    squares.push_back(new object(rsquare, psquare));

}

bool cooldown = false;
int help = 0;
void handleInput() {

    if (glfwGetKey(stevieRender->window, GLFW_KEY_R) == GLFW_PRESS) {
        //reset position
        physics->reset();
    }
    if (glfwGetKey(stevieRender->window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        //reset position
        physics->stop();
    }

    double xpos, ypos;
    glfwGetCursorPos(stevieRender->window, &xpos, &ypos);
    xpos = ((xpos / stevieRender->width) * 2) - 1.0f;
    ypos = ((ypos / stevieRender->height) * 2) - 1.0f;
    ypos = -ypos;
    xpos *= stevieRender->getZoomLevel();
    ypos *= stevieRender->getZoomLevel();

    if (glfwGetMouseButton(stevieRender->window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && !cooldown) {
        spawnCube((vector(xpos, ypos) - startPosition).normalize() );
        cooldown = true;
        help = 0;
    }
    if(cooldown)
     help++;
    if (help > 50)
        cooldown = false;
}

void renderImGui(){
    //begin frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

 
    ImGui::Text("Stevie Physics");


    const char* demosItems[] = { "Projectile Motion", "Pendulum" };
    static const char* demo_current_item = demosItems[0];

    
    if (ImGui::BeginCombo("##combo", demo_current_item)) // The second parameter is the label previewed before opening the combo.
    {
        for (int n = 0; n < IM_ARRAYSIZE(demosItems); n++)
        {
            bool is_selected = (demo_current_item == demosItems[n]); // You can store your selection however you want, outside or inside your objects
            if (ImGui::Selectable(demosItems[n], is_selected)) {
                demo_current_item = demosItems[n];
                if (n == 0)
                    currentDemo = PROJECTILE;
                if (n == 1)
                    currentDemo = PENDULUM;
            }

            if (is_selected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
    
    if (currentDemo == PENDULUM) {

    }

    if (currentDemo == PROJECTILE) {

        ImGui::SliderFloat("velocity", &velocity, 1.0f, 10.0f);

        if (physics->useAirResistance)
            ImGui::SliderFloat("k", &k, 0.0f, 5.0f);

        if (ImGui::Button("Air resistance"))
            physics->useAirResistance = !physics->useAirResistance;


        IntegrationScheme currentIntegrationScheme;

        const char* intSchemeitems[] = { "EXPLICIT_EULER", "ANALYTIC" };
        static const char* intScheme_current_item = intSchemeitems[0];


        if (ImGui::BeginCombo("##combo", intScheme_current_item)) // The second parameter is the label previewed before opening the combo.
        {
            for (int n = 0; n < IM_ARRAYSIZE(intSchemeitems); n++)
            {
                bool is_selected = (intScheme_current_item == intSchemeitems[n]); // You can store your selection however you want, outside or inside your objects
                if (ImGui::Selectable(intSchemeitems[n], is_selected)) {
                    intScheme_current_item = intSchemeitems[n];
                    if (n == 0)
                        physics->integrationScheme = EXPLICIT_EULER;
                    if (n == 1)
                        physics->integrationScheme = ANALYTIC;
                }

                if (is_selected) {
                    ImGui::SetItemDefaultFocus();
                }
                // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
            }
            ImGui::EndCombo();
        }
    }
    //end frame
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


void appRenderLoop() {

    handleInput();

    physics->simulate();

    for(int i = 0; i < squares.size(); i++)
        squares[i]->update();
    
    renderImGui();
}

int main()
{
    stevieRender->init();

    initScene();

    initImGui();

    physics->init();
    stevieRender->enterRenderLoop(&appRenderLoop);

}

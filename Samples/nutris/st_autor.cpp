#include "st_author.h"
#include "st_conf.h"
#include "CStateManager.h"

void ST_Author::onMouseDown(int x, int y, int b)
{
    console->echo( Nutmeg::format("myApp::onMouseDown::SwitchStateTo - %s\n", "main menu") );
    stmgr->setNextState(game::STATE_MAINMENU);
}//onMouseDown

void ST_Author::onRender()
{
    render->begin();

    render->setColor(Nutmeg::vec3(1.0f, 1.0f, 1.0f), 1.0f);
    render->clear(true, true);

    camera->getCamera().setView(render);
    scene->render(camera->getCamera(), render);

    render->end();
}

void ST_Author::onShutdown()
{
    onDeactivateState();
}

void ST_Author::onStartup(int argc, const char **argv)
{
    initSystemVariables(engine);
}

void ST_Author::onUpdate(float dt)
{
    if (scene) scene->update(dt);
}

void ST_Author::onActivateState()
{
    scene = new Nutmeg::Scene(engine);
    scene->load("../Resource/game/scn_author.xml_scene");
    camera = (Nutmeg::CameraNode*)scene->getNode("camera");
    scene->setActiveCamera( camera );
    camera->getCamera().setView(render);
    isActivated = true;
}

void ST_Author::onDeactivateState()
{
    if (!isActivated) return;

    delete scene;
    scene = NULL;
    isActivated = false;
}

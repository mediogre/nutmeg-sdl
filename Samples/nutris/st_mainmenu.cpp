#include "st_mainmenu.h"
#include "st_conf.h"
#include "CStateManager.h"

void ST_MainMenu::onMouseDown(int x, int y, int b)
{
    Nutmeg::Line ray = camera->getCamera().getRay(x,y);
    Nutmeg::TracePoint tp;
    Nutmeg::Node* n = scene->trace(ray, tp, true, false);
    if (!n) return;
    console->message( Nutmeg::format("myApp::onMouseDown::RayTraced -> %s <- node.\n", n->getName()) );

     if ( strcmp(n->getName(), "but_start")==0 )
     {
        console->echo( Nutmeg::format("myApp::onMouseDown::SwitchStateTo - %s\n", "game") );
        stmgr->setNextState(game::STATE_GAME);
     }
     else if ( strcmp(n->getName(), "but_about")==0 )
     {
        console->echo( Nutmeg::format("myApp::onMouseDown::SwitchStateTo - %s\n", "about") );
        stmgr->setNextState(game::STATE_ABOUT);
     }
     else if ( strcmp(n->getName(), "but_exit")==0 )
     {
        console->echo( Nutmeg::format("myApp::onMouseDown::SwitchStateTo - %s\n", "exit") );
        stmgr->setNextState(game::STATE_NONE);
     }
}//onMouseDown

void ST_MainMenu::onMouseMove(int x, int y)
{
    mx = x;
    my = y;
}

void ST_MainMenu::onRender()
{
    render->begin();

    render->setColor(Nutmeg::vec3(1.0f, 1.0f, 1.0f), 1.0f);
    render->clear(true, true);

    camera->getCamera().setView(render);
    scene->render(camera->getCamera(), render);

    render->end();
}

void ST_MainMenu::onShutdown()
{
    onDeactivateState();
}

void ST_MainMenu::onStartup(int argc, const char **argv)
{
    initSystemVariables(engine);
}

void ST_MainMenu::onUpdate(float dt)
{
    Nutmeg::Line ray = camera->getCamera().getRay(mx,my);
    Nutmeg::TracePoint tp;
    Nutmeg::Node* n = scene->trace(ray, tp, true, false);

    if ( n )
    {
        if ( strcmp(n->getName(), "but_start")==0 ) rotateStart = 3;
        else if ( strcmp(n->getName(), "but_about")==0 ) rotateAbout = 3;
        else if ( strcmp(n->getName(), "but_exit")==0 ) rotateExit = 3;
    }

	if (rotateStart>0)
	{
        n = scene->getNode("but_start");
        Nutmeg::quat q = n->getRot();
        Nutmeg::vec3 a = q.getAngles();
        q.set(a+Nutmeg::vec3(rotateStart*dt, 0, rotateStart*dt));
        n->setRot(q);
        rotateStart -= 0.005;
	}

	if (rotateAbout>0)
	{
        n = scene->getNode("but_about");
        Nutmeg::quat q = n->getRot();
        Nutmeg::vec3 a = q.getAngles();
        q.set(a+Nutmeg::vec3(rotateAbout*dt, 0, rotateAbout*dt));
        n->setRot(q);
        rotateAbout -= 0.005;
	}

	if (rotateExit>0)
	{
        n = scene->getNode("but_exit");
        Nutmeg::quat q = n->getRot();
        Nutmeg::vec3 a = q.getAngles();
        q.set(a+Nutmeg::vec3(rotateExit*dt, 0, rotateExit*dt));
        n->setRot(q);
        rotateExit -= 0.005;
	}

	if (scene) scene->update(dt);
}

void ST_MainMenu::onActivateState()
{
    scene = new Nutmeg::Scene(engine);
    scene->load("../Resource/game/scn_mainmenu.xml_scene");
    camera = (Nutmeg::CameraNode*)scene->getNode("camera");
    scene->setActiveCamera( camera );
    camera->getCamera().setView(render);

    rotateStart = -1;
    rotateAbout = -1;
    rotateExit = -1;

    isActivated = true;
}

void ST_MainMenu::onDeactivateState()
{
    if (!isActivated) return;

    delete scene;
    scene = NULL;
    isActivated = false;
}

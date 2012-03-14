#include "st_game.h"
#include "st_conf.h"
#include "CStateManager.h"


// обработчик событий
void ST_Game::onKeyDown(int key)
{
    if (key == Nutmeg::KEY_LEFT)
    {
        if (game->checkLeft()) game->moveLeft();
	}
    else if (key == Nutmeg::KEY_RIGHT)
    {
        if (game->checkRight()) game->moveRight();
	}
    else if (key == Nutmeg::KEY_UP)
    {
        game->shapeRotate();
	}
    else if (key == Nutmeg::KEY_DOWN)
    {
        game->shapeAccelerate(true);
	}
}

void ST_Game::onMouseDown(int x, int y, int b)
{
    Nutmeg::Line ray = camera->getCamera().getRay(x,y);
    Nutmeg::TracePoint tp;
    Nutmeg::Node* n = scene->trace(ray, tp, true, false);
    if (!n) return;
    console->message( Nutmeg::format("myApp::onMouseDown::RayTraced -> %s <- node.\n", n->getName()) );

     if ( strcmp(n->getName(), "but_exit")==0 )
     {
        console->echo( Nutmeg::format("myApp::onMouseDown::SwitchStateTo - %s\n", "exit") );
        stmgr->setNextState(game::STATE_MAINMENU);
     }
}//onMouseDown


void ST_Game::onRender()
{
    render->begin();

    render->setColor(Nutmeg::vec3(1.0f, 1.0f, 1.0f), 1.0f);
    render->clear(true, true);

    camera->getCamera().setView(render);
    scene->render(camera->getCamera(), render);

    render->end();
}

void ST_Game::onShutdown()
{
    onDeactivateState();
}

void ST_Game::onStartup(int argc, const char **argv)
{
    initSystemVariables(engine);
}

void ST_Game::onUpdate(float dt)
{
    if (scene) scene->update(dt);
    if (game) game->update(dt);
}

void ST_Game::onActivateState()
{
    scene = new Nutmeg::Scene(engine);
    scene->load("../Resource/game/scn_game.xml_scene");
    camera = (Nutmeg::CameraNode*)scene->getNode("camera");
    scene->setActiveCamera( camera );
    camera->getCamera().setView(engine->getRender());
    Nutmeg::Log::message( "State Game activated.\n" );

    game = new Game(scene);
    game->startNew();

    isActivated = true;
}

void ST_Game::onDeactivateState()
{
    if (!isActivated) return;

    delete game;
    game = NULL;

    delete scene;
    scene = NULL;

    isActivated = false;
}

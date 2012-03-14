#include "game.h"

/*
    конструктор класса, заполняем поля значениями по умолчанию и
    загружаем ресурсы игрового окна
*/
Game::Game(Nutmeg::Scene *scn)
{
    scene = scn;

    cube.load("Core/Primitives/pBox.mesh");
    trg_stock = scene->getNode("trig_stakan");

    Nutmeg::Material* mat;

    for(int i=0; i<8; ++i)
    {
        mat = new Nutmeg::Material();
        mat->diffuse.load( Nutmeg::format("Game/cube_%d.jpg", i) );
        cube_mat[i] = scene->appendMaterial(mat);
    }

    clearStock();
    shapeClear(cells);

    Nutmeg::vec3f t = trg_stock->getPos();

	for(int z=0, zmax=20; z<zmax; ++z)
	{
        for(int x=0, xmax=10; x<xmax; ++x)
        {
            createCube( getCellPos(x,z), 7);
        }
	}
}
Nutmeg::vec3f Game::getCellPos(float x, float y)
{
    return Nutmeg::vec3f((x + 0.5f) / float(10) - 0.5f, 0.0f, 0.5f - (y + 0.5f) / float(20));
}

/*
    инициализация новой игры
*/
void Game::startNew()
{
    // значения по умолчанию
	score=0;
	level=1;
	accelerate = false;
	speed=1;
	timer=0;

	clearStock(true); // очищаем стакан
    shapeClear(cells, true);

	bGame=true; // игра запущена
	shapeNew(); // создаем новую фигуру вверху стакана
}

Nutmeg::Node* Game::createCube( Nutmeg::vec3f pos, int col)
{
	Nutmeg::MeshNode* tmp = new Nutmeg::MeshNode(scene);
	tmp->setMesh(cube);

    if (col==7)
    {
        tmp->setScale(Nutmeg::vec3f(0.5, 0.5, 0.5));
    }

	tmp->setParent(trg_stock);
    tmp->setPos(pos);
    tmp->setMaterial(cube_mat[col]);

    return tmp;
}

/*
    заполняем матрицу в соответствии с запрошенным типом
*/
void Game::shapeFill(Nutmeg::Node* c[4][4], ShapeType tip, int col)
{
    shapeClear(c);

	switch(tip)
	{
        case line:
            c[0][1] = createCube( getCellPos(1,0), col);
            c[1][1] = createCube( getCellPos(1,1), col);
            c[2][1] = createCube( getCellPos(1,2), col);
            c[3][1] = createCube( getCellPos(1,3), col);
        break;

        case square:
            c[0][0] = createCube( getCellPos(0,0), col);
            c[0][1] = createCube( getCellPos(1,0), col);
            c[1][0] = createCube( getCellPos(0,1), col);
            c[1][1] = createCube( getCellPos(1,1), col);
        break;

        case leftL:
            c[0][0] = createCube( getCellPos(0,0), col);
            c[0][1] = createCube( getCellPos(1,0), col);
            c[0][2] = createCube( getCellPos(2,0), col);
            c[1][0] = createCube( getCellPos(0,1), col);
        break;

        case rightL:
            c[0][0] = createCube( getCellPos(0,0), col);
            c[0][1] = createCube( getCellPos(1,0), col);
            c[0][2] = createCube( getCellPos(2,0), col);
            c[1][2] = createCube( getCellPos(2,1), col);
        break;

        case pyramide:
            c[0][0] = createCube( getCellPos(0,0), col);
            c[0][1] = createCube( getCellPos(1,0), col);
            c[0][2] = createCube( getCellPos(2,0), col);
            c[1][1] = createCube( getCellPos(1,1), col);
        break;

        case leftZ:
            c[0][0] = createCube( getCellPos(0,0), col);
            c[0][1] = createCube( getCellPos(1,0), col);
            c[1][1] = createCube( getCellPos(1,1), col);
            c[1][2] = createCube( getCellPos(2,1), col);
        break;

        case rightZ:
            c[0][1] = createCube( getCellPos(1,0), col);
            c[0][2] = createCube( getCellPos(2,0), col);
            c[1][0] = createCube( getCellPos(0,1), col);
            c[1][1] = createCube( getCellPos(1,1), col);
        break;
	}
}//shapeFill

/*
    создает новую фигуру вверху стакана
*/
void Game::shapeNew()
{
    curX = 3;
    curY = 0;

    Nutmeg::math::randomize();

    color = Nutmeg::math::random(6);
    curType = ShapeType(Nutmeg::math::random(7));

    shapeFill(cells, curType, color);
    for( int i=0; i<4; i++)
        for( int j=0; j<4; j++)
            if (cells[i][j] != 0)
            {
                 Nutmeg::vec3f p = cells[i][j]->getPos();
                 p.x += 0.3;
                 cells[i][j]->setPos(p);
            }
}

/*
    вращает падающую фигуру
*/
void Game::shapeRotate()
{
    if (curType==square) return; // исключение: квадрат не вращаем

    int curXX = curX;

    bool tempShape[4][4];
	shapeClear(tempShape);

	switch(curType)
	{
        case line:
            // ислючение: линию перекладываем
			if(cells[0][0] != 0)
			{
				for(int k=0; k<4; k++) tempShape[k][1]=true;
			}
			else
			{
				for(int k=0; k<4; k++) tempShape[0][k]=true;
                if (curX==-1) curXX=0;
                else if (curX>7) curXX=6;
			}
		break;

		default:
            //переворачиваем фигуру квадрат 3х3 во временную матрицу
            // 123    360    654
            // 456 -> 250 -> 321
            // 000    140    000
            if ( !cells[2][0] && !cells[2][1] && !cells[2][2] )
            {
                tempShape[0][0]=(cells[0][2]?true:false);
                tempShape[1][0]=(cells[0][1]?true:false);
                tempShape[2][0]=(cells[0][0]?true:false);
                tempShape[0][1]=(cells[1][2]?true:false);
                tempShape[1][1]=(cells[1][1]?true:false);
                tempShape[2][1]=(cells[1][0]?true:false);

                tempShape[0][2]=false;
                tempShape[1][2]=false;
                tempShape[2][2]=false;
            }
            else
            {
                tempShape[0][0]=(cells[0][1]?true:false);
                tempShape[0][1]=(cells[1][1]?true:false);
                tempShape[0][2]=(cells[2][1]?true:false);
                tempShape[1][0]=(cells[0][0]?true:false);
                tempShape[1][1]=(cells[1][0]?true:false);
                tempShape[1][2]=(cells[2][0]?true:false);

                tempShape[2][0]=false;
                tempShape[2][1]=false;
                tempShape[2][2]=false;
            }

            //патчик, если матрица ушла на 2 ед. за край стакана
            if (curX==-1) curXX=0;
            else if (curX==8) curXX=7;
	}

    bool tempStock[4][4];
    shapeClear(tempStock);

    //забираем область "стакана" на которую ляжет перевенутая фигура
    for(int y=0; y<4; y++)
    {
        int my = y+curY;

        for(int x=0; x<4; x++)
        {
            int mx = x+curXX;

            if (mx > -1 && mx < 10) // если матрица фигуры вся в стакане
                tempStock[y][x] = (stock[my][mx]==0?false:true);
            else // если матрица фигуры вышла за пределы стакана
                tempStock[y][x] = true;
        }
    }

    //сравниваем матрицы
    if (curType==line) //для линии 4х4
    {
        for(int y=0; y<4; y++)
            for(int x=0; x<4; x++)
                // матрицы пересеклись, поворот запрещен
                if (tempStock[y][x] && tempShape[y][x]) return; //выходим
    }
    else // для остальных фигур 3х3
    {
        for(int y=0; y<3; y++)
            for(int x=0; x<3; x++)
                // матрицы пересеклись, поворот запрещен
                if (tempStock[y][x] && tempShape[y][x]) return; //выходим
    }

    //помещаем в стакан повернутую фигуру
    shapeClear(cells, true);
    for(int y=0; y<4; y++)
        for(int x=0; x<4; x++)
            if (tempShape[y][x])
                cells[y][x] = createCube(getCellPos(x+curXX,y+curY), color);
    curX = curXX;
}//shapeRotate

/*
    очищает матрицу фигуры
*/
void Game::shapeClear( Nutmeg::Node* c[4][4], bool tryRemove )
{
	for( int i=0; i<4; i++)
		for( int j=0; j<4; j++)
		{
		    if (tryRemove && c[i][j] != 0) scene->remove(c[i][j]);
			c[i][j] = 0;//createCube(Nutmeg::vec3f(j,i,0), 7);
		}
}

void Game::shapeClear( bool c[4][4] )
{
	for( int i=0; i<4; i++)
		for( int j=0; j<4; j++)
		{
			c[i][j] = false;
		}
}
/*
    включает или выключает ускорение см. update()
*/
void Game::shapeAccelerate(bool a)
{
    accelerate=a;
}

/*
    ощищает "стакан"
*/
void Game::clearStock(bool tryRemove)
{
	for(int i=0; i<20; i++)
		for(int j=0; j<10; j++)
		{
		    if (tryRemove && stock[i][j] != 0) scene->remove(stock[i][j]);
			stock[i][j] = 0;
		}
}

/*
    проверка достижения дна фигурой
*/
bool Game::IsAtBottom()
{
	for(int i=0; i<4; i++)
		for(int j=0; j<4; j++)
			if(cells[i][j] != 0)
			{
				if((curY+i)>=19 || stock[curY+i+1][curX+j] != 0) return true;
			}

	return false;
}

/*
    удаление полных линий, обновление счетчика очков и
    запуск спецэффетов
*/
void Game::checkForLine()
{
	bool flag;

	for(int i=0; i<20; i++)
	{
		flag=true;
		for(int j=0; j<10; j++) if ( stock[i][j] == 0 ) flag = false;

		if(flag==true)
		{
			score++;

			if (score%10==0) levelUp();

            for(int j=0; j<10; j++) scene->remove(stock[i][j]);

			for(int k=i; k>0; k--)
				for(int j=0; j<10; j++)
				{
					stock[k][j]=stock[k-1][j];
					if (stock[k][j] != 0) moveDown(stock[k][j]);
				}
		}
	}
}

/*
    проверка возможности на движение фигуры вправо
*/
bool Game::checkRight()
{
	for(int i=0; i<4; i++)
		for(int j=0; j<4; j++)
			if(cells[i][j] != 0)
			{
				if((curX+j)>=9 || stock[curY+i][curX+j+1] != 0) return false;
			}

	return true;
}

/*
    проверка возможности на движение фигуры влево
*/
bool Game::checkLeft()
{
	for(int i=0; i<4; i++)
		for(int j=0; j<4; j++)
			if(cells[i][j] != 0)
			{
				if((curX+j)<=0 || stock[curY+i][curX+j-1] != 0) return false;
			}

	return true;
}

/*
    сдвигает фигуру вниз
*/
void Game::moveDown()
{
	for( int i=0; i<4; i++)
    for( int j=0; j<4; j++)
	if (cells[i][j] != 0)
	{
	    Nutmeg::vec3f pos = cells[i][j]->getPos();
	    pos.z -= 0.05;
	    cells[i][j]->setPos(pos);
	}
}

void Game::moveDown(Nutmeg::Node* n, float p)
{
    Nutmeg::vec3f pos = n->getPos();
    pos.z -= p;
    n->setPos(pos);
}

/*
    сдвигает фигуру влево
*/
void Game::moveLeft()
{
	for( int i=0; i<4; i++)
    for( int j=0; j<4; j++)
	if (cells[i][j] != 0)
	{
	    Nutmeg::vec3f pos = cells[i][j]->getPos();
	    pos.x += -0.1;
	    cells[i][j]->setPos(pos);
	}
    curX-=1;
}

/*
    сдвигает фигуру вправо
*/
void Game::moveRight()
{
	for( int i=0; i<4; i++)
    for( int j=0; j<4; j++)
	if (cells[i][j] != 0)
	{
	    Nutmeg::vec3f pos = cells[i][j]->getPos();
	    pos.x += 0.1;
	    cells[i][j]->setPos(pos);
	}
	curX+=1;
}

/*
    повышение уровня скорости
    снижаем по 0.1 с 1 до 0.1
    снижаем по 0.01 с 0.1 до 0
*/
void Game::levelUp()
{
    if (speed<=0.1) speed -= 0.01f;
    else speed -= 0.1f;

    if (speed<0.01) speed=0.01f;
    else level++;
}

/*
    понижение уровня скорости
    повышаеем зеркально понижению levelUP
*/
void Game::levelDown()
{
    if (speed<0.1) speed += 0.01;
    else speed += 0.1;

    if (speed>1.f) speed=1;
    else level--;

}

/*
    обновление состояния

    параметр delta - прошедшее кол-во милисекунд между вызовами update
*/
bool Game::update(const float delta)
{
    /*
        если включено ускорение двигаем вниз с максимальной скоростью

        если прошло количество секунд равное скорости speed
        то обнуляем timer и двигаем фигуру вниз, иначе увеличиваем timer
        на величину delta  и выходим
    */
    timer += delta;
    if (accelerate)
    {
        if (timer < 0.1) return true;
    }
    else
    {
        if (timer < speed ) return true;
    }
    timer = 0;

    if(IsAtBottom()) // фигура достигла дна
    {
        for(int i=0; i<4; i++)
            for(int j=0; j<4; j++)
                if(cells[i][j] != 0)
                {
                    stock[curY+i][curX+j] = cells[i][j];
                }

        shapeNew(); // создаем новую фигуру

        if(IsAtBottom()) return false;

        checkForLine();
        accelerate=false;
        return true;
    }

    moveDown();
    curY += 1;
    return true;
}

#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include "Nutmeg.h"

// типы фигур
enum ShapeType { line = 0, square, rightL, leftL, pyramide, leftZ, rightZ };

class Game
{
    public:
        Game(Nutmeg::Scene *scn);

        void startNew(); //инициализирует и стартует новую игру
        bool update(const float delta); // обновляет состояние игры, если ложь - игра закончена

        void shapeRotate(); // вращает фигуру
        void shapeAccelerate(bool a); //вкл. ускоренного падения фигуры
        void moveDown();
        void moveLeft();
        void moveRight();

        bool checkLeft(); // проверяет возможность движения влево
        bool checkRight(); // проверяет возможность движения вправо

    private:
        Nutmeg::Scene *scene;

        Nutmeg::MeshRef cube; // модель кубика
        int cube_mat[8]; // текстуры с цветами кубиков

        Nutmeg::Node* cells[4][4]; // 4x4 матрица для хранения фигуры
        ShapeType	curType;     // текущая фигура
        int color;              // цвет текущей фигуры

        Nutmeg::Node* trg_stock; // триггер родитель для кубиков, в локальном пространстве которого будем играть
        Nutmeg::Node* stock[20][10]; // матрица "стакана"
        int	    curY; // положение фигуры по Y
        int	    curX; // положение фигуры по X
        bool	bGame; // если false то игра закончена
        int	    score; // здесь храним набранные очки
        int     level; // здесь храним уровень скорости падения фигур

        bool accelerate; // вкл/выкл ускоренного падения фигуры
        float speed; // текущая скорость падения
        float timer; // в связке со speed не дает фигуре моментально упасть на дно см. update()

        Nutmeg::vec3f getCellPos(float x, float y);

        void shapeNew(); // создает новую фигуру вверху "стакана"
        void shapeClear( Nutmeg::Node* c[4][4], bool tryRemove = false); //обнуляет матрицу текущей фигуры
        void shapeClear( bool c[4][4] ); //обнуляет матрицу текущей фигуры
        void shapeFill( Nutmeg::Node* c[4][4], ShapeType tip, int col ); // заполняет матрицу фигуры указанным типом формы
        Nutmeg::Node* createCube( Nutmeg::vec3f pos, int col);

        void levelUp(); // поднимает уровень скорости падения
        void levelDown(); // снижает уровень скорости падения

        void moveDown(Nutmeg::Node* n, float p = 0.05);

        void checkForLine(); // удаляет полные строки
        // добавляет эффект взрыва для удаляемой строки

        bool IsAtBottom(); // проверка достижения дна фигурой

        void clearStock(bool tryRemove = false); // очищает "стакан"
};

#endif // GAME_H_INCLUDED

#include "action.hh"

MyActionInitialization::MyActionInitialization()
{

}

MyActionInitialization::~MyActionInitialization()
{

}

void MyActionInitialization::Build() const
{
    // apply the generator and the run action to the programm
    
    MyPrimaryGenerator *generator = new MyPrimaryGenerator();
    SetUserAction(generator);

    MyRunAction *runAction = new MyRunAction();
    SetUserAction(runAction);
}

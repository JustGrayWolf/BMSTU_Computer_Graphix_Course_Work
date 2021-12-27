#include "Primitiv.h"
#include "INCLUDS.h"
int List::get_npoint(){return cur->npoint;};
int List::get_nnorm(){return cur->nnorm;};
int List::get_ntext(){return cur->ntext;};
void List::add(int i , int j, int l)
{
    if (head == nullptr)
    {
        head = new elem();
        tell = head;
        cur = head;
    }
    else
    {
        tell->nextel = new elem();
        tell = tell->nextel;
    }
    tell->npoint = i;
    tell->nnorm = j;
    tell->ntext = l;
};

bool List::is_end(){return (cur == tell);};
bool List::is_begin(){return (cur == head);};
void List::to_end(){cur = tell;};
void List::to_begin(){cur = head;};
void List::next(){if (cur != nullptr && cur != tell) cur = cur->nextel;};
const List List::operator++()
{if (cur != nullptr && cur != tell) cur = cur->nextel;};
void Point3D::print()
{
    std::cout << x << ' ' << y << ' ' << z << "; ";
}

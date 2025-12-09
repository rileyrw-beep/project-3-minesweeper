Name: Riley Nelson
Section: #20682
UFL email: riley.nelson@ufl.edu
System: WSL
Compiler: Clang
SFML version: 2.5
IDE: CLion

Other notes: So I tried some of the new things we've learned this year. 
Like polymorphic interfaces with inheritance, 
function pointers, functors, lambdas, std::function,
sets, maps, stuff like that.

I created a tile interface (abstract base class) where RegularTile and Mine inherit from.

I did this so I could create a board of smart pointers to tiles that don't need to know if they
are a mine or not. You know, for that nice polymorphism. I don't think that was necessary now,
but I did it anyways. 

I also tried to use function stuff like functors and lambdas; I do not think I used them in the
conventionally necessary ways which you guys were pushing for, but I used them good in some places
I think.

I also made it so that you can click anywhere on the board and it will be valid on your first click.
If there was a mine or adjacent mine I moved it away. :)
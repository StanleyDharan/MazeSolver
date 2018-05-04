#include "maze.h"
#include <list>
#include <iterator>
#include <iostream>

list<Coord> pathHistory; // used in capacity of FILO, data is transfered to Path array before return.
list<Coord> coordsToUnmark; // kept so any dead end paths are block, unmarked at the end before findPath() returns.
int prevSize = 0;
int runNum = 0;

//bool isDeadEnd(Coord curPos, const Coord& end, Maze& theMaze){
//    // if there is no avaliable route to take you are at a dead end.
//    if (!patherFinder(theMaze, curPos.x + 1, curPos.y, end) && !patherFinder(theMaze, curPos.x - 1, curPos.y, end) && !patherFinder(theMaze, curPos.x, curPos.y - 1, end) && !patherFinder(theMaze, curPos.x, curPos.y + 1, end))
//        return true;
//    else return false;
//}


Coord backTrack(){ // recalls to intersection where dead-end route was found
    if (!pathHistory.empty() && pathHistory.size() > 1){ // safe check to ensure list wont cause a crash if an element is removed.
        Coord deadEnd = pathHistory.back();
        coordsToUnmark.push_back(deadEnd);
        pathHistory.pop_back();
    }
    return pathHistory.front();
}


int patherFinder(Maze& theMaze, int x, int y, const Coord& end){
    Coord currentPos(x, y);
    //are we at the the end of the maze? 
    if (currentPos == end)
    {
        theMaze.mark(currentPos);
        pathHistory.push_back(currentPos);
        return pathHistory.size();
    }
    //is position empty?
    if (!theMaze.isEmpty(currentPos))
    {
        return 0;
    }
    //are we at the bourders of the maze?
    if (currentPos.x == 0 || currentPos.x == theMaze.width()){
        return 0;
    }

    theMaze.mark(currentPos); // added to solution, possibly on the right path
    pathHistory.push_back(currentPos);

    // check for next possible route:

    //Check spot above current position
    if (patherFinder(theMaze, x - 1, y, end)){
        return pathHistory.size();
    }
    //Check spot below current position
    if (patherFinder(theMaze, x + 1, y, end)){
        return pathHistory.size();
    }
    //Check spot left current position
    if (patherFinder(theMaze, x, y - 1, end)){
        return pathHistory.size();
    }
    //Check spot right current position
    if (patherFinder(theMaze, x, y + 1, end)){
        return pathHistory.size();
    }
    // is you are at a dead end
    if (!patherFinder(theMaze, x + 1, y, end) && !patherFinder(theMaze, x - 1, y, end) && !patherFinder(theMaze, x, y - 1, end) && !patherFinder(theMaze, x, y + 1, end)){
        Coord lastGoodPos = backTrack();
        return patherFinder(theMaze, lastGoodPos.x, lastGoodPos.y, end);
    }
    return 0;
}


int findPath (Maze& theMaze, const Coord& start, const Coord& end, Coord path[])
{
    Coord position;
    patherFinder(theMaze, start.x, start.y, end);


    if (pathHistory.size() == 1 && pathHistory.front().x != end.x && pathHistory.front().y != end.y){ // If only 1 Coord is added to list by the algorithm and it is NOT equal to the end (meaning maze cannot be finished)
        
        for (int i = 0; i < theMaze.height(); i++){ // ensures no positions are marked if there is no end to the maze
            for (int j = 0; j < theMaze.width(); j++){
                position.set(j, i);
                theMaze.unMark(position);
            }
        }
        return 0;

    }
    else {

        list<Coord>::iterator it;
        int ii = 0;

        for (it = pathHistory.begin(); it != pathHistory.end(); it++){

                path[ii] = *it; // Transfering data from wrapper function List to Path array
                ii++;
        }

        for (it = coordsToUnmark.begin(); it != coordsToUnmark.end(); it++){ // unmarking all the dead end Coords

            theMaze.unMark(*it);

        }
        prevSize = ii;
        coordsToUnmark.clear(); // clearing for any future calls
        pathHistory.clear();   // ^ same as above ^

        runNum++;
        return ii;
    }
}
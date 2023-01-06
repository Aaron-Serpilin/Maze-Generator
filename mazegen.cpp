#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <stack>
#include <utility>

class initialMazeGenerator {
    public:
        void setRows (int rows) {numRows = rows;}
        void setColumns (int columns) {numColumns = columns;}
        std::vector<std::vector<char> > mazeGenerator ();
    
    private:
        int numRows;
        int numColumns;
};

std::vector<std::vector<char> > initialMazeGenerator::mazeGenerator () {

    std::vector<std::vector<char> > initialMaze;
    int numVerticalCharacters = (numRows * 2) + 1;
    int numHorizontalCharacters = (numColumns * 4) + 1;

    for (int i = 0; i < numVerticalCharacters; i++) {

        initialMaze.push_back(std::vector<char> ());

        for (int j = 0; j < numHorizontalCharacters; j++) {

            if ((i == 0 && j == 0) || (i % 2 == 0 && j % 4 == 0)) {
                initialMaze.at(i).push_back('+');
            } else if ((i % 2 != 0 && j % 4 == 0)) {
                initialMaze.at(i).push_back('|');
            } else if ((i == 0 || i % 2 == 0) && (j % 4 != 0)) {
                initialMaze.at(i).push_back('-');
            } else {
                initialMaze.at(i).push_back(' ');
            }
        }
    }

    return initialMaze;

}

class randomMazeGenerator {
    public:
        void setRows (int rows) {numRows = rows;}
        void setColumns (int columns) {numColumns = columns;}
        void setInitialMaze (std::vector<std::vector<char> > maze) {initialMaze = maze;}
        void setInitialCoordinates (std::pair<int, int> initialCoordinates) {visitedCellsStack.push(initialCoordinates); }
        bool isNeighborValid (std::pair<int, int> startingCoordinates);
        std::vector<std::pair<int, int> > neighborGenerator (std::pair<int, int> startingCoordinates);
        std::pair<int, int> wallOpeningGenerator ();
        std::vector<std::vector<char> > mazePathGenerator();

    private:
        int numRows;
        int numColumns;
        std::vector<std::vector<char> > initialMaze;
        std::vector<std::pair<int, int> > visitedCells;
        std::stack<std::pair<int, int> > visitedCellsStack;
};

bool randomMazeGenerator::isNeighborValid (std::pair<int, int> startingCoordinates) {

    int xCoordinate = startingCoordinates.first;
    int yCoordinate = startingCoordinates.second;

    if (xCoordinate < 0 || xCoordinate >= numRows || yCoordinate < 0 || yCoordinate >= numColumns) { 
        return false;
    }

    for (int i = 0; i < visitedCells.size(); i++) {
        
        if (xCoordinate == visitedCells.at(i).first && yCoordinate == visitedCells.at(i).second) {
            return false;
        }
    }
    
    return true;
}

std::vector<std::pair<int, int> > randomMazeGenerator::neighborGenerator (std::pair<int, int> startingCoordinates) {

    std::vector<std::pair<int, int> > neighborCells;
    int xCoordinate = startingCoordinates.first;
    int yCoordinate = startingCoordinates.second;

    if (isNeighborValid((std::make_pair(xCoordinate + 1, yCoordinate)))) {neighborCells.push_back(std::make_pair(xCoordinate + 1, yCoordinate));}
    if (isNeighborValid((std::make_pair(xCoordinate -1 , yCoordinate)))) {neighborCells.push_back(std::make_pair(xCoordinate - 1, yCoordinate));}
    if (isNeighborValid((std::make_pair(xCoordinate, yCoordinate + 1)))) {neighborCells.push_back(std::make_pair(xCoordinate, yCoordinate + 1));}
    if (isNeighborValid((std::make_pair(xCoordinate, yCoordinate -1)))) {neighborCells.push_back(std::make_pair(xCoordinate, yCoordinate - 1));}

    return neighborCells;

}

std::pair<int, int> randomMazeGenerator::wallOpeningGenerator () {

    std::pair<int, int> removeWallLocations; 

    int xStartingCoordinate = visitedCellsStack.top().first;
    int yStartingCoordinate = visitedCellsStack.top().second;

    visitedCellsStack.pop();

    int xBacktrackCoordinate = visitedCellsStack.top().first;
    int yBacktrackCoordinate = visitedCellsStack.top().second;

    //The transformation below transforms the coordinates from the theoretical cell locations to the visual maze locations for the walls
    int xTransformedStartingLocation = (xStartingCoordinate * 2) + 1;
    int yTransformedStartingLocation = (yStartingCoordinate * 4) + 2;
    int xTransformedBacktrackLocation = (xBacktrackCoordinate * 2) + 1;
    int yTransformedBacktrackLocation = (yBacktrackCoordinate * 4) + 1;

    int xRemovedWallLocation = (xTransformedStartingLocation + xTransformedBacktrackLocation) / 2;
    int yRemovedWallLocation = ((yTransformedStartingLocation + yTransformedBacktrackLocation) / 2) + 1;

    removeWallLocations.first = xRemovedWallLocation;
    removeWallLocations.second = yRemovedWallLocation;

    return removeWallLocations;

}

std::vector<std::vector<char> > randomMazeGenerator::mazePathGenerator () {

    std::vector<std::vector<char> > randomMaze = initialMaze;

    int numCells = numRows * numColumns;

    while (visitedCells.size() <= numCells - 1) { //This makes sure the number of removed walls equals the number of cells

        int xPosition = visitedCellsStack.top().first;
        int yPosition = visitedCellsStack.top().second;

        std::vector<std::pair<int, int> > neighborCells = neighborGenerator(std::make_pair(xPosition, yPosition));

        if (neighborCells.size() == 0) {

            std::pair<int, int> removeWallLocations = wallOpeningGenerator();
            int xRemovedWallLocation = removeWallLocations.first;
            int yRemovedWallLocation = removeWallLocations.second;

            if (xPosition != xRemovedWallLocation) { //Checks if the change is in y to remove the three '-' characters
                randomMaze.at(xRemovedWallLocation).at(yRemovedWallLocation) = ' ';
                randomMaze.at(xRemovedWallLocation).at(yRemovedWallLocation + 1) = ' ';
                randomMaze.at(xRemovedWallLocation).at(yRemovedWallLocation - 1) = ' ';
            } else { //Checks if the change is in x to only remove the '|' character
                randomMaze.at(xRemovedWallLocation).at(yRemovedWallLocation) = ' ';
            }
 
        } else {

            int randomSeed = (rand() % neighborCells.size());
            visitedCells.push_back(neighborCells.at(randomSeed));
            visitedCellsStack.push(neighborCells.at(randomSeed));

        }
    }

    while (visitedCellsStack.size() > 1) {

        int xPosition = visitedCellsStack.top().first;
        int yPosition = visitedCellsStack.top().second;

        std::pair<int, int> removeWallLocations = wallOpeningGenerator();
        int xRemovedWallLocation = removeWallLocations.first;
        int yRemovedWallLocation = removeWallLocations.second;

        if (yPosition != yRemovedWallLocation) { //Checks if the change is in y to remove the three '-' characters
            randomMaze.at(xRemovedWallLocation).at(yRemovedWallLocation) = ' ';
            randomMaze.at(xRemovedWallLocation).at(yRemovedWallLocation + 1) = ' ';
            randomMaze.at(xRemovedWallLocation).at(yRemovedWallLocation - 1) = ' ';
        } else { //Checks if the change is in x to only remove the '|' character
            randomMaze.at(xRemovedWallLocation).at(yRemovedWallLocation) = ' ';
        }

    }

    return randomMaze;
}

class mazeSolver {
    public:
        void setRows (int rows) {numRows = rows;}
        void setColumns (int columns) {numColumns = columns;}
        void setMazeToBeSolved (std::vector<std::vector<char> > maze) {mazeToBeSolved = maze;}
        bool isNeighborValid (std::pair<int, int> startingCoordinates, int xModificationValue, int yModificationValue);
        std::vector<std::pair<int, int> > neighborGenerator (std::pair<int, int> startingCoordinates);
        bool findPath (std::pair<int, int> initialCoordinates, std::pair<int, int> finalCoordinates);
        
    private:
        int numRows;
        int numColumns;
        std::vector<std::vector<char> > mazeToBeSolved;
};

bool mazeSolver::isNeighborValid (std::pair<int, int> startingCoordinates, int xModificationValue, int yModificationValue) {

    int xCoordinate = startingCoordinates.first;
    int yCoordinate = startingCoordinates.second;
    int xTransformedCoordinate = (xCoordinate * 2) + 1;
    int yTransformedCoordinate = (yCoordinate * 4) + 2;

    if (xCoordinate < 0 || xCoordinate >= numRows || yCoordinate < 0 || yCoordinate >= numColumns) { 
        return false;
    }

    //Based on the coordinate change between the current position and the possible neighbor, I check if there is an opening where there should be one to determine if it is a valid move
    if (xModificationValue == 1 && yModificationValue == 0) {

        if (mazeToBeSolved.at(xTransformedCoordinate - 1).at(yTransformedCoordinate) != ' ') {
            return false;
        }

    } else if (xModificationValue == -1 && yModificationValue == 0) {

        if (mazeToBeSolved.at(xTransformedCoordinate + 1).at(yTransformedCoordinate) != ' ') {
            return false;
        }

    } else if (xModificationValue == 0 && yModificationValue == 1) {

        if (mazeToBeSolved.at(xTransformedCoordinate).at(yTransformedCoordinate - 2) != ' ') {
            return false;
        }

    } else if (xModificationValue == 0 && yModificationValue == -1) {

        if (mazeToBeSolved.at(xTransformedCoordinate).at(yTransformedCoordinate + 2) != ' ') {
            return false;
        }

    }
    
    return true;
}

std::vector<std::pair<int, int> > mazeSolver::neighborGenerator (std::pair<int, int> startingCoordinates) {

    std::vector<std::pair<int, int> > neighborCells;
    int xCoordinate = startingCoordinates.first;
    int yCoordinate = startingCoordinates.second;

    if (isNeighborValid((std::make_pair(xCoordinate + 1, yCoordinate)), 1, 0)) {neighborCells.push_back(std::make_pair(xCoordinate + 1, yCoordinate));}
    if (isNeighborValid((std::make_pair(xCoordinate -1 , yCoordinate)), -1, 0)) {neighborCells.push_back(std::make_pair(xCoordinate - 1, yCoordinate));}
    if (isNeighborValid((std::make_pair(xCoordinate, yCoordinate + 1)), 0, 1)) {neighborCells.push_back(std::make_pair(xCoordinate, yCoordinate + 1));}
    if (isNeighborValid((std::make_pair(xCoordinate, yCoordinate -1)), 0, -1)) {neighborCells.push_back(std::make_pair(xCoordinate, yCoordinate - 1));}

    return neighborCells;

}

bool mazeSolver::findPath (std::pair<int, int> initialCoordinates, std::pair<int, int> finalCoordinates) {

    int xPosition = initialCoordinates.first;
    int yPosition = initialCoordinates.second;
    int xTransformedPosition = (xPosition * 2) + 1;
    int yTransformedPosition = (yPosition * 4) + 2;

    mazeToBeSolved.at(xTransformedPosition).at(yTransformedPosition) = '.';

    if (initialCoordinates == finalCoordinates) {

        for (int i = 0; i < mazeToBeSolved.size(); i++) {
            for (int j = 0; j < mazeToBeSolved.at(i).size(); j++) {
                std::cout << mazeToBeSolved.at(i).at(j);
            }
            std::cout << std::endl;
        }

        return true;

    } else {
        std::vector<std::pair<int, int> > neighborCells = neighborGenerator(std::make_pair(xPosition, yPosition)); //I generate all neighbors that have an opening
        

        for (int i = 0; i < neighborCells.size(); i++) {

            if (mazeToBeSolved.at((neighborCells.at(i).first * 2) + 1).at((neighborCells.at(i).second * 4) + 2) == ' ') {//Now I only pick the neighbor with an opening that has not been visited
                
                int xNewPosition = neighborCells.at(i).first;
                int yNewPosition =neighborCells.at(i).second;

                bool success = findPath(std::make_pair(xNewPosition, yNewPosition), std::make_pair(numRows - 1, numColumns -1));

                if (success) {
                    return true;
                }
            }
        }

        mazeToBeSolved.at(xTransformedPosition).at(yTransformedPosition) = ' ';
    }

    return false;

}

int main (int argc, char* argv[]) {
    try {

        if (argc < 3) {
            throw std::invalid_argument("invalid input");
        }

        int numRows = std::stoi(argv[1]);
        int numColumns = std::stoi(argv[2]);
        srand(time(0));

        if (numRows < 1 || numColumns < 1) {
            throw std::invalid_argument("invalid input");
        }

        if (argc == 4) {
            srand((std::stoi(argv[3])));
        } 

        initialMazeGenerator initialMaze;
        initialMaze.setRows(numRows);
        initialMaze.setColumns(numColumns);

        randomMazeGenerator randomMaze;
        randomMaze.setRows(numRows);
        randomMaze.setColumns(numColumns);
        randomMaze.setInitialMaze(initialMaze.mazeGenerator());
        randomMaze.setInitialCoordinates(std::make_pair(0, 0));

        mazeSolver mazeToBeSolved;
        mazeToBeSolved.setRows(numRows);
        mazeToBeSolved.setColumns(numColumns);
        mazeToBeSolved.setMazeToBeSolved(randomMaze.mazePathGenerator());
        mazeToBeSolved.findPath(std::make_pair(0, 0), std::make_pair(numRows - 1, numColumns - 1));

    } catch (std::invalid_argument &excpt) {
        std::cout << "invalid input" << std::endl;
    }

    return 0;
    
}
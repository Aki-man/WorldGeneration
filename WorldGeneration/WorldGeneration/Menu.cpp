#include "Menu.h"
#include "VectorMap.h"

std::string Menu::getSaveName()
{
    system("cls");
    std::vector<std::string> currentSaves = SaveSystemHelper::GetSaves();
    std::unordered_map<std::string, int> saveMap = SaveSystemHelper::GetSaveMap();
    std::string userInput;
    while (true) {
        std::cout << "These are the currently existing saves:\n";
        for (int i = 0; i < currentSaves.size(); ++i) {
            std::cout << i + 1 << ":" << currentSaves[i] << std::endl;
        }
        std::cout << "What would you like to call the new save?\n";

        std::cin >> userInput;

        if (saveMap.find(userInput) != saveMap.end()) {
            system("cls");
            std::cout << "That save already exists!\n";
        }
        else {
            break;
        }
    }
    return userInput;
}

void Menu::PlayerMenu(std::string name, Player& newPlayer) {
    while (true) {
        newPlayer.world.increaseChunkUse();
        newPlayer.getViewWithShadows();
        newPlayer.cleanUpView();
        newPlayer.world.cleanUnusedChunks();

        std::cout << newPlayer << std::endl;
        std::cout << "please press a WASD button, q to quit, save to save" << std::endl;
        std::string entry;
        std::cin >> entry;
        if (entry == "w" || entry == "W")
            newPlayer.moveUp();
        else if (entry == "s" || entry == "S")
            newPlayer.moveDown();
        else if (entry == "a" || entry == "A")
            newPlayer.moveLeft();
        else if (entry == "d" || entry == "D")
            newPlayer.moveRight();
        else if (entry == "q")
            break;
        else if (entry == "save") {
            newPlayer.save(name);
        }
        else
            continue;
        system("cls");
    }
}

void Menu::ParallelPlayerMenu(std::string name, ParallelPlayer& newPlayer) {
    while (true) {
        newPlayer.world.increaseChunkUse();
        newPlayer.getViewWithShadows();
        newPlayer.cleanUpView();
        newPlayer.world.cleanUnusedChunks();

        std::cout << newPlayer << std::endl;
        std::cout << "please press a WASD button, q to quit, save to save" << std::endl;
        std::string entry;
        std::cin >> entry;
        if (entry == "w" || entry == "W")
            newPlayer.moveUp();
        else if (entry == "s" || entry == "S")
            newPlayer.moveDown();
        else if (entry == "a" || entry == "A")
            newPlayer.moveLeft();
        else if (entry == "d" || entry == "D")
            newPlayer.moveRight();
        else if (entry == "q")
            break;

        else if (entry == "save") {
            newPlayer.save(name);
        }
        else if (entry == "dj")
            newPlayer.currentCoordinate = Coordinate(newPlayer.currentCoordinate.x + 50, newPlayer.currentCoordinate.y);
        else if (entry == "aj")
            newPlayer.currentCoordinate = Coordinate(newPlayer.currentCoordinate.x - 50, newPlayer.currentCoordinate.y);
        else if (entry == "wj")
            newPlayer.currentCoordinate = Coordinate(newPlayer.currentCoordinate.x, newPlayer.currentCoordinate.y - 50);
        else if (entry == "sj")
            newPlayer.currentCoordinate = Coordinate(newPlayer.currentCoordinate.x, newPlayer.currentCoordinate.y + 50);
        else
            continue;
        system("cls");
    }
}

void Menu::generateNewIslandMenu(std::string name) {

    while (true) {
        system("cls");
        std::cout << "Please pick one of the following:" << std::endl;
        std::cout << "1: Generate a serial four island map" << std::endl;
        std::cout << "2: Generate a serial one island map" << std::endl;
        std::cout << "3: Generate a parallel four island map" << std::endl;
        std::cout << "4: Generate a parallel one island map" << std::endl;

        std::string userInput = "";
        std::cin >> userInput;

        if (userInput == "q") {
            break;
        }

        std::cout << "Please pick a size for the generated map:" << std::endl;
        std::string sizeInput;
        std::cin >> sizeInput;
        int size = 0;
        try {
            size = std::stoi(sizeInput);
        }
        catch (const std::invalid_argument) {
            std::cout << "Invalid size parameter." << std::endl;
            std::cin >> sizeInput;
            if (userInput != "q")
                continue;
        }

        if (size < 50)
            size = 50;

        if (userInput == "1") {
            WorldMap *map = new WorldMap(size, size);
            
            (*map).GenerateFourIslandMap();
            (*map).name = name;
            (*map).save();
            delete map;
            ChunkedMap playMap = ChunkedMap(size, size, name);
            Player newPlayer(Coordinate(size / 2, size / 2), playMap, 20);
            newPlayer.save(name);
            PlayerMenu(name, newPlayer);
        }
        else if (userInput == "2") {
            WorldMap *map = new WorldMap(size, size);
            
            (*map).GenerateOneIslandMap();
            (*map).name = name;
            (*map).save();
            delete map;
            ChunkedMap playMap = ChunkedMap(size, size, name);
            Player newPlayer(Coordinate(size / 2, size / 2), playMap, 20);
            newPlayer.save(name);
            PlayerMenu(name, newPlayer);
        }
        if (userInput == "3") {
            VectorMap *pmap = new VectorMap(size, size);
            
           
            (*pmap).GenerateFourIslandMap();
            (*pmap).name = name;
            (*pmap).parallelSave();
            delete pmap;
            ChunkedMap playMap = ChunkedMap(size, size, name);
            ParallelPlayer newPlayer(Coordinate(size / 2, size / 2), playMap, 20);
            newPlayer.save(name);

            ParallelPlayerMenu(name, newPlayer);
        }
        else if (userInput == "4") {
            VectorMap* pmap = new VectorMap(size, size);
            
            (*pmap).GenerateOneIslandMap();
            (*pmap).name = name;
            (*pmap).parallelSave();
            delete pmap;
            ChunkedMap playMap = ChunkedMap(size, size, name);
            ParallelPlayer newPlayer(Coordinate(size / 2, size / 2), playMap, 20);
            newPlayer.save(name);
            ParallelPlayerMenu(name, newPlayer);
        }

    }
}

void Menu::loadIslandMenu() {

    while (true) {
        system("cls");
        std::cout << "Please pick one of the following:" << std::endl;
        std::vector<std::string> saves = SaveSystemHelper::GetSaves();
        for (int i = 0; i < saves.size(); ++i) {
            std::cout << i + 1 << ":" << saves[i] << std::endl;
        }

        std::string userInput = "";

        std::cin >> userInput;

        if (userInput == "q") {
            break;
        }
        int convertedInput = 0;
        try {
            convertedInput = std::stoi(userInput);
        }
        catch (std::exception e) {
            continue;
        }

        if (convertedInput > saves.size()) {
            continue;
        }

        int n = SaveSystemHelper::GetSaveSize(saves[convertedInput - 1]);
        //VectorMap map = VectorMap(n, n);
        //map.load(saves[convertedInput - 1]);
        //WorldMap smap = map;
        ChunkedMap playMap = ChunkedMap(n, n, saves[convertedInput - 1]);
        ParallelPlayer newPlayer(Coordinate(0, 0), playMap, 20);
        newPlayer.load(saves[convertedInput - 1]);
        playMap.loadChunk(playMap.getChunkId(newPlayer.currentCoordinate));
        ParallelPlayerMenu(saves[convertedInput - 1], newPlayer);
    }

}

void Menu::testsMenu() {

    while (true) {
        system("cls");
        std::cout << "Please pick one of the following:" << std::endl;
        std::cout << "1: Run generation tests" << std::endl;
        std::cout << "2: Run view tests" << std::endl;
        std::cout << "3: Run save tests" << std::endl;
        std::cout << "4: Run one island generation tests" << std::endl;
        std::cout << "5: Run all tests" << std::endl;
        std::cout << "6: Run serial save tests" << std::endl;

        std::string userInput = "";
        std::cin >> userInput;

        if (userInput == "1") {
            int sizeInput = 0;
            std::cout << "Input island size for generation test:" << std::endl;
            std::cin >> sizeInput;

            system("cls");
            Tests::VectorGenerationTest(sizeInput);
            std::cout << "Enter any key to continue" << std::endl;
            std::cin >> userInput;
        }
        else if (userInput == "2") {
            int sizeInput = 0;
            std::cout << "Input view size for view test:" << std::endl;
            std::cin >> sizeInput;

            system("cls");
            Tests::ParallelViewTest(sizeInput);
            std::cout << "Enter any key to continue" << std::endl;
            std::cin >> userInput;
        }
        else if (userInput == "3") {
            int sizeInput = 0;
            std::cout << "Input island size for save test:" << std::endl;
            std::cin >> sizeInput;

            system("cls");
            Tests::SaveWorldToFileTest(sizeInput, "testFile");
            std::cout << "Enter any key to continue" << std::endl;
            std::cin >> userInput;
        }
        else if (userInput == "4") {
            int sizeInput = 0;
            std::cout << "Input island size for generation test:" << std::endl;
            std::cin >> sizeInput;

            system("cls");
            Tests::ParallelOneIslandGenerationTest(sizeInput);
            std::cout << "Enter any key to continue" << std::endl;
            std::cin >> userInput;
        }
        else if (userInput == "5") {
            int islandSizeInput = 0;
            std::cout << "Input island size for batch test:" << std::endl;
            std::cin >> islandSizeInput;

            int viewSizeInput = 0;
            std::cout << "Input view size for batch test:" << std::endl;
            std::cin >> viewSizeInput;

            system("cls");
            Tests::BatchTests(islandSizeInput, viewSizeInput, "testFile");
            std::cout << "Enter any key to continue" << std::endl;
            std::cin >> userInput;
        }
        else if (userInput == "6") {
            int sizeInput = 0;
            std::cout << "Input island size for save test:" << std::endl;
            std::cin >> sizeInput;

            system("cls");
            Tests::SaveWorldToFileTestSerial(sizeInput, "testFile");
            std::cout << "Enter any key to continue" << std::endl;
            std::cin >> userInput;
        }
        else if (userInput == "q") {
            break;
        }
    }
}

void Menu::playMenu() {
    while (true) {
        system("cls");
        
        std::vector<std::string> saves = SaveSystemHelper::GetSaves();
        if (saves.size() == 0)
            std::cout << "There are no currently existing saves, press n to create a new save\n";
        else {
            std::cout << "Please pick one of the following:" << std::endl;
            for (int i = 0; i < saves.size(); ++i) {
                std::cout << i + 1 << ":" << saves[i] << std::endl;
            }
            std::cout << "Or press n to create a new save\n";
        }

        std::string userInput = "";

        std::cin >> userInput;

        if (userInput == "q") {
            break;
        }
        else if (userInput == "n") {
            std::string mapName = getSaveName();
            generateNewIslandMenu(mapName);
            continue;
        }
        int convertedInput = 0;
        try {
            convertedInput = std::stoi(userInput);
        }
        catch (std::exception e) {
            continue;
        }

        if (convertedInput > saves.size()) {
            continue;
        }

        int n = SaveSystemHelper::GetSaveSize(saves[convertedInput - 1]);
        //VectorMap map = VectorMap(n, n);
        //map.load(saves[convertedInput - 1]);
        //WorldMap smap = map;
        ChunkedMap playMap = ChunkedMap(n, n, saves[convertedInput - 1]);
        ParallelPlayer newPlayer(Coordinate(0, 0), playMap, 20);
        newPlayer.load(saves[convertedInput - 1]);
        playMap.loadChunk(playMap.getChunkId(newPlayer.currentCoordinate));
        ParallelPlayerMenu(saves[convertedInput - 1], newPlayer);
    }
}

void Menu::mainMenu() {

    while (true) {
        system("cls");
        std::cout << "Welcome to the random world generation app!" << std::endl;
        std::cout << "Please pick one of the following:" << std::endl;
        std::cout << "1: Play" << std::endl;
        std::cout << "2: Run some tests" << std::endl;

        std::string userInput = "";
        std::cin >> userInput;

        if (userInput == "1") {
            playMenu();
        }
        else if (userInput == "2") {
            testsMenu();
        }
        else if (userInput == "q") {
            break;
        }

    }


}


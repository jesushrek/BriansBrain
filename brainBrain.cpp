#include <raylib.h>
#include <algorithm>

//#define WRAP_AROUND

constexpr int g_Height { 200 };
constexpr int g_Width { 200 };
constexpr int g_Size { 4 };

enum State{ 
    Dead,
    Dying,
    Alive,
};

Color colors[] = { 
    [Dead] = BLACK,
    [Dying] = SKYBLUE,
    [Alive] = BLUE,
};

void drawGrid(int* grid) 
{ 
    for(int i = 0; i < g_Height; ++i)
    { 
        for(int j = 0; j < g_Width; ++j)
        { 
            int idx { i * g_Width + j };
            Color color { colors[ grid[idx]] };
            if(grid[idx] != Dead)
                DrawRectangle(j * g_Size, i * g_Size, g_Size, g_Size, color);
        }
    }
}

int countNeighbor(int y, int x, int* grid)
{ 
    int neighbor{};

    for(int i = -1; i <= 1; ++i)
    { 
        for(int j = -1; j <= 1; ++j)
        { 
            if(j == 0 && i == 0) continue;

#ifndef WRAP_AROUND
            int dy { (y + i) };
            int dx { (x + j) };
#endif

#ifdef WRAP_AROUND

            int dy { (y + i + g_Height) % g_Height };
            int dx { (x + j + g_Width) % g_Width };
            if(dx >= g_Width || dy >= g_Height || dy < 0 || dx < 0)
               continue;
#endif

            int index { dy * g_Width + dx };
            if(grid[index] == Alive)
                ++neighbor;
        }
    }

    return neighbor;
}

void update(int* grid, int* newGrid)
{ 
    for(int i = 0; i < g_Height; ++i)
    { 
        for(int j = 0; j < g_Width; ++j)
        { 
            int neighbor { countNeighbor(i, j, grid) };
            int index { i * g_Width + j };

            if(grid[index] == Dead && neighbor == 2)
                newGrid[index] = Alive;
            else if(grid[index] == Alive)
                newGrid[index] = Dying;
            else
                newGrid[index] = Dead;
        }
    }
}

int main() 
{ 
    InitWindow(g_Width * g_Size, g_Height * g_Size, "briansBrain");
    SetTargetFPS(10);

    int* grid { new int[g_Height * g_Width]() };
    int* newGrid { new int[g_Height * g_Width]() };

    for(int i = 0; i < g_Width * g_Height; ++i)
    {
        if(!GetRandomValue(0, 9)) 
        {  
            grid[i] = Alive;
        }
    }

    while(!WindowShouldClose())
    {
        update(grid, newGrid);
        BeginDrawing();
        ClearBackground(BLACK);
        drawGrid(grid);
        EndDrawing();
        std::swap(grid, newGrid);
    }

    delete[] grid;
    delete[] newGrid;
    CloseWindow();
    return 0; 
}

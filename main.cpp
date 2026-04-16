#include <raylib.h>
using namespace std;

const int win_width = 800;
const int win_height = 800;
Texture2D pieces[32];
Image image_array[32];

struct Piece
{
    int temp_x;
    int temp_y;
    int index;
    Texture2D tex;

    void Piece_create(int x, int y, Texture2D texture, int i)
    {
        temp_x = x;
        temp_y = y;
        tex = texture;
        index = i;
    }
};

Piece array_piece[32]; // 32 objects for 32 pieces of Chess each with their own coordinates on board, texture and an index(to help computer identify which piece is which)

// VOID INITIALISE TEXTURES WILL BE CALLED EVERYTIME THE LOOP RUNS AND IT WILL SET ALL THE PIECES AT THE  RIGHT SPOT
void initialise_textures(int x)
{
    ///// THIS WILL RUN ONLY ONCE BECAUSE WE NEED TO RESIZE TEXTURES ONLY FOR THE FIRST TIME  //////
    if (x == 1)
    {
        for (int i = 0; i < 32; i++)
        {
            image_array[i] = LoadImageFromTexture(pieces[i]);
            UnloadTexture(pieces[i]);
            ImageResize(&image_array[i], 60, 60);
            pieces[i] = LoadTextureFromImage(image_array[i]);
            UnloadImage(image_array[i]);
            if (i <= 7)
            {
                DrawTexture(pieces[i], 20 + (100 * (i)), 20, PURPLE);
                array_piece[i].tex = pieces[i];
            }
            else if (i > 7 && i < 16)
            {
                DrawTexture(pieces[i], 20 + (100 * (i - 8)), 20 + (100), PURPLE);
                array_piece[i].tex = pieces[i];
            }
            else if (i >= 16 && i < 24)
            {
                DrawTexture(pieces[i], 20 + (100 * (i - 16)), 20 + (600), BLUE);
                array_piece[i].tex = pieces[i];
            }
            else if (i >= 24 && i < 32)
            {
                DrawTexture(pieces[i], 20 + (100 * (i - 24)), 20 + (700), BLUE);
                array_piece[i].tex = pieces[i];
            }
        }
    }
    ////////////////////         DRAWING ALL PIECES AT THE RIGHT LOCATION            /////////////////////
    else
    {
        for (int i = 0; i < 32; i++)
        {
            DrawTexture(array_piece[i].tex, array_piece[i].temp_x, array_piece[i].temp_y, WHITE);
        }
    }
}

int abs(int x)
{
    if (x > 0)
    {
        return x;
    }
    else
    {
        return (-1) * x;
    }
}

// DRAW BOARD, THIS WILL SET THE BOARD TILES
void draw_board()
{
    int pos_X = 0;
    int pos_Y = 0;

    for (int i = 0; i < 8; i++)
    {
        int pos_X = 0;

        for (int j = 0; j < 8; j++)
        {
            if ((i + j) % 2 == 0)
            {
                DrawRectangle(pos_X, pos_Y, 100, 100, WHITE);
                pos_X += 100;
            }
            else
            {
                DrawRectangle(pos_X, pos_Y, 100, 100, LIME);
                pos_X += 100;
            }
        }
        pos_Y += 100;
    }
    pos_Y = 0;
}

// MOUSE_INPUT HIGHLIGHTS THE TILE ON WHICH THE MOUSE IS HOVERING
void mouse_input()
{
    int mouse_X = GetMouseX();
    int mouse_Y = GetMouseY();
    int temp_x, temp_y;
    if (mouse_X % 100 == 0)
    {
        temp_x = mouse_X;
    }
    else
    {
        for (int i = mouse_X; i % 100 > 0; i--)
        {
            temp_x = i - 1;
        }
    }
    if (mouse_Y % 100 == 0)
    {
        temp_y = mouse_Y;
    }
    else
    {
        for (int j = mouse_Y; j % 100 > 0; j--)
        {
            temp_y = j - 1;
        }
    }

    DrawRectangle(temp_x, temp_y, 100, 100, PINK);
    // DONT ASK ME WHY THIS IS PINK //
}

void pawnpromotioncheck()
// Pawn will be automatically be promoted to Queen
{
    for (int i = 8; i < 16; i++)
    {
        if (array_piece[i].temp_y == 720)
        {
            array_piece[i].tex = array_piece[3].tex;
            array_piece[i].index = 3;
        }
    }
    for (int i = 16; i < 24; i++)
    {
        if (array_piece[i].temp_y == 20)
        {
            array_piece[i].tex = array_piece[27].tex;
            array_piece[i].index = 27;
        }
    }
}

int gameover()
{
    if (array_piece[4].temp_x == 1000)
    {
        return 0;
    }
    else if (array_piece[28].temp_x == 1000)
    {
        return 1;
    }
    else
    {
        return 2;
    }
}

int main()
{
    Image icon = LoadImage("res/w_knight_2x_ns.png");
    InitWindow(win_width, win_height, "Chess using Raylib by Irtaza and Taha");
    SetWindowIcon(icon);
    SetTargetFPS(60);
    UnloadImage(icon);
    InitAudioDevice();
    Music opening = LoadMusicStream("res/sounds/chill_guy.mp3");
    Sound move_piece = LoadSound("res/sounds/move_piece.mp3");    
    Sound cap_piece = LoadSound("res/sounds/cap_piece.mp3");
    Sound checkmate = LoadSound("res/sounds/checkmate audio.mp3");
    Sound castling = LoadSound("res/sounds/castle.mp3");
    Sound pawn_promote = LoadSound("res/sounds/ARA ARA.mp3");
    Sound load_time_opener = LoadSound("res/load_time_opener.mp3");
    PlaySound(load_time_opener);
    PlayMusicStream(opening);

    /////////////////////////////////       LOADING GRAPHICS    ////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////

    pieces[0] = LoadTexture("res/b_rook_2x_ns.png");
    array_piece[0].Piece_create(20, 20, pieces[0], 0);
    pieces[1] = LoadTexture("res/b_knight_2x_ns.png");
    array_piece[1].Piece_create(120, 20, pieces[1], 1);
    pieces[2] = LoadTexture("res/b_bishop_2x_ns.png");
    array_piece[2].Piece_create(220, 20, pieces[2], 2);
    pieces[3] = LoadTexture("res/b_queen_2x_ns.png");
    array_piece[3].Piece_create(320, 20, pieces[3], 3);
    pieces[4] = LoadTexture("res/b_king_2x_ns.png");
    array_piece[4].Piece_create(420, 20, pieces[4], 4);
    pieces[5] = LoadTexture("res/b_bishop_2x_ns.png");
    array_piece[5].Piece_create(520, 20, pieces[5], 5);
    pieces[6] = LoadTexture("res/b_knight_2x_ns.png");
    array_piece[6].Piece_create(620, 20, pieces[6], 6);
    pieces[7] = LoadTexture("res/b_rook_2x_ns.png");
    array_piece[7].Piece_create(720, 20, pieces[7], 7);
    pieces[8] = LoadTexture("res/b_pawn_2x_ns.png");
    array_piece[8].Piece_create(20, 120, pieces[8], 8);
    for (int i = 0; i < 7; i++)
    {
        pieces[9 + i] = LoadTexture("res/b_pawn_2x_ns.png");
        array_piece[9 + i].Piece_create(120 + (100 * i), 120, pieces[9 + i], 9 + i);
    }

    pieces[24] = LoadTexture("res/w_rook_2x_ns.png");
    array_piece[24].Piece_create(20, 720, pieces[24], 24);
    pieces[25] = LoadTexture("res/w_knight_2x_ns.png");
    array_piece[25].Piece_create(120, 720, pieces[25], 25);
    pieces[26] = LoadTexture("res/w_bishop_2x_ns.png");
    array_piece[26].Piece_create(220, 720, pieces[26], 26);
    pieces[27] = LoadTexture("res/w_queen_2x_ns.png");
    array_piece[27].Piece_create(320, 720, pieces[27], 27);
    pieces[28] = LoadTexture("res/w_king_2x_ns.png");
    array_piece[28].Piece_create(420, 720, pieces[28], 28);
    pieces[29] = LoadTexture("res/w_bishop_2x_ns.png");
    array_piece[29].Piece_create(520, 720, pieces[29], 29);
    pieces[30] = LoadTexture("res/w_knight_2x_ns.png");
    array_piece[30].Piece_create(620, 720, pieces[30], 30);
    pieces[31] = LoadTexture("res/w_rook_2x_ns.png");
    array_piece[31].Piece_create(720, 720, pieces[31], 31);
    pieces[16] = LoadTexture("res/w_pawn_2x_ns.png");
    array_piece[16].Piece_create(20, 620, pieces[16], 16);
    for (int i = 0; i < 7; i++)
    {
        pieces[17 + i] = LoadTexture("res/w_pawn_2x_ns.png");
        array_piece[17 + i].Piece_create(120 + (100 * i), 620, pieces[17 + i], 17 + i);
    }

    //////////////////////////////////   ORIGINAL TEXTURES LOADED   //////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////

    int x = 1;
    int big_x, big_y, diff_x, diff_y;
    bool is_whites_turn = true;
    int b_rook1, b_rook2, b_king, w_rook1, w_rook2, w_king = 0; // FOR CASTLING
    int index1, index2 = 35;                                // any number other than 0-32 would have worked here.
    bool isvalidmove = false;

    Vector2 initialPosition = {0, 0};
    Vector2 finalPosition = {0, 0};

    bool hasInitialPosition = false;
    bool hasFinalPosition = false;

    while (WindowShouldClose() == false)
    {

        UpdateMusicStream(opening);

        BeginDrawing();

        draw_board();

        initialise_textures(x);
        x++;

        mouse_input();

        // Capture the initial position with the left mouse button
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            PlaySound(move_piece);
            initialPosition = GetMousePosition();
            while (int(initialPosition.x) % 100 != 0)
            {
                initialPosition.x--;
            }
            while (int(initialPosition.y) % 100 != 0)
            {
                initialPosition.y--;
            }

            initialPosition.y += 20;
            initialPosition.x += 20;
            hasInitialPosition = true;
            hasFinalPosition = false; // RESETS FINAL POSITION
        }

        ///////   Highlights the selected box  ////////
        if (hasInitialPosition)
        {
            for (int i = 0; i < 32; i++)
            {
                if (array_piece[i].temp_x == initialPosition.x && array_piece[i].temp_y == initialPosition.y)
                {
                    DrawRectangle(initialPosition.x - 20, initialPosition.y - 20, 100, 100, YELLOW);
                    DrawTexture(array_piece[i].tex, array_piece[i].temp_x, array_piece[i].temp_y, WHITE);
                }
            }
        }

        // Capture the final position with the right mouse button
        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
        {
            PlaySound(move_piece);
            finalPosition = GetMousePosition();
            while (int(finalPosition.x) % 100 != 0)
            {
                finalPosition.x--;
            }
            finalPosition.x += 20;
            while (int(finalPosition.y) % 100 != 0)
            {
                finalPosition.y--;
            }
            finalPosition.y += 20;
            hasFinalPosition = true;
            // Now we have both an initial position and a final position //
        }

        ///// CHECK IF A WHITE PIECE IS KILLING ANOTHER WHITE PIECE AND SAME FOR BLACK ///////

        for (int i = 0; i < 32; i++)
        {
            if (array_piece[i].temp_x == initialPosition.x && array_piece[i].temp_y == initialPosition.y)
            {
                index1 = array_piece[i].index;
            }
            if (array_piece[i].temp_x == finalPosition.x && array_piece[i].temp_y == finalPosition.y)
            {
                index2 = array_piece[i].index;
            }
        }
        if (index1 == 35 || index2 == 35)
        {
            isvalidmove = true;
        }
        else if (index1 < 16 && index2 >= 16)
        {
            isvalidmove = true;
        }
        else if (index2 < 16 && index1 >= 16)
        {
            isvalidmove = true;
        }
        else
        {
            isvalidmove = false;
        }

        if (isvalidmove)
        {
            /////////////////  PAWN VALID MOVE CHECKPOINT (FOR BLACK PAWNS)  ////////////////
            if (index1 > 7 && index1 < 16)
            {
                if (((finalPosition.x == initialPosition.x) || ((finalPosition.x == initialPosition.x - 100) && (index2 > 15) && (index2 < 32)) || ((finalPosition.x == initialPosition.x + 100) && (index2 > 15) && (index2 < 32))) && ((finalPosition.y == initialPosition.y + 100) || ((finalPosition.y == initialPosition.y + 200) && (initialPosition.y == 120) && (initialPosition.x == finalPosition.x))))
                {
                    isvalidmove = true;
                }
                else
                {
                    isvalidmove = false;
                }
            }
            /////////////////    PAWN VALID MOVE CHECKPOINT (FOR WHITE PAWNS)  ///////////////
            else if (index1 >= 16 && index1 < 24)
            {
                if (((finalPosition.x == initialPosition.x) || ((finalPosition.x == initialPosition.x - 100) && (index2 < 16)) || ((finalPosition.x == initialPosition.x + 100) && (index2 < 16))) && ((finalPosition.y == initialPosition.y - 100) || ((finalPosition.y == initialPosition.y - 200) && (initialPosition.y == 620) && (initialPosition.x == finalPosition.x))))
                {
                    isvalidmove = true;
                }
                else
                {
                    isvalidmove = false;
                }
            }
            /////////////////////   ROOK VALID MOVE CHECKPOINT  ///////////////////////
            else if (index1 == 0 || index1 == 7 || index1 == 24 || index1 == 31)
            {
                if (initialPosition.x == finalPosition.x || initialPosition.y == finalPosition.y)
                {
                    isvalidmove = true;
                }
                else
                {
                    isvalidmove = false;
                }
            }
            /////////////////////   KNIGHT VALID MOVE CHECKPOINT //////////////////////
            else if (index1 == 1 || index1 == 6 || index1 == 25 || index1 == 30)
            {
                if (((abs(initialPosition.x - finalPosition.x) == 200) && (abs(initialPosition.y - finalPosition.y) == 100)) || ((abs(initialPosition.x - finalPosition.x) == 100) && (abs(initialPosition.y - finalPosition.y) == 200)))
                {
                    isvalidmove = true;
                }
                else
                {
                    isvalidmove = false;
                }
            }
            /////////////////////   BISHOP VALID MOVE CHEKPOINT ///////////////////////
            else if (index1 == 2 || index1 == 5 || index1 == 26 || index1 == 29)
            {
                if (abs(initialPosition.x - finalPosition.x) == abs(initialPosition.y - finalPosition.y))
                {
                    isvalidmove = true;
                }
                else
                {
                    isvalidmove = false;
                }
            }
            /////////////////////   QUEEN VALID MOVE CHECKPOINT ///////////////////////
            else if (index1 == 3 || index1 == 27)
            {
                if ((initialPosition.x == finalPosition.x || initialPosition.y == finalPosition.y) || (abs(initialPosition.x - finalPosition.x) == abs(initialPosition.y - finalPosition.y)))
                {
                    isvalidmove = true;
                }
                else
                {
                    isvalidmove = false;
                }
            }
            /////////////////////   KING VALID MOVE CHECKPOINT  ///////////////////////
            else if (index1 == 4 || index1 == 28)
            {
                if (((abs(initialPosition.x - finalPosition.x) == 100) || (abs(initialPosition.x - finalPosition.x) == 0)) && ((abs(initialPosition.y - finalPosition.y) == 100) || (abs(initialPosition.y - finalPosition.y) == 0)))
                {
                    isvalidmove = true;
                }
                // CASTLING
                else if (index1 == 4 && index2 == 35 && b_rook1 == 0 && b_king == 0 && initialPosition.x == 420 && initialPosition.y == 20 && finalPosition.x == 220 && finalPosition.y == 20)
                {
                    for (int i = 0; i < 32; i++)
                    {
                        if (array_piece[i].temp_y == 20 && (array_piece[i].temp_x == 120 || array_piece[i].temp_x == 220 || array_piece[i].temp_x == 320))
                        {
                            isvalidmove = false;
                        }
                    }
                    if (isvalidmove)
                    {
                        array_piece[0].temp_x = 320;
                        PlaySound(castling);
                    }
                }
                else if (index1 == 4 && index2 == 35 && b_rook2 == 0 && b_king == 0 && initialPosition.x == 420 && initialPosition.y == 20 && finalPosition.x == 620 && finalPosition.y == 20)
                {
                    for (int i = 0; i < 32; i++)
                    {
                        if (array_piece[i].temp_y == 20 && (array_piece[i].temp_x == 520 || array_piece[i].temp_x == 620))
                        {
                            isvalidmove = false;
                        }
                    }
                    if (isvalidmove)
                    {
                        array_piece[7].temp_x = 520;
                        PlaySound(castling);
                    }
                }
                else if (index1 == 28 && index2 == 35 && w_rook1 == 0 && w_king == 0 && initialPosition.x == 420 && initialPosition.y == 720 && finalPosition.x == 220 && finalPosition.y == 720)
                {
                    for (int i = 0; i < 32; i++)
                    {
                        if (array_piece[i].temp_y == 720 && (array_piece[i].temp_x == 120 || array_piece[i].temp_x == 220 || array_piece[i].temp_x == 320))
                        {
                            isvalidmove = false;
                        }
                    }
                    if (isvalidmove)
                    {
                        array_piece[24].temp_x = 320;
                        PlaySound(castling);
                    }
                }
                else if (index1 == 28 && index2 == 35 && w_rook2 == 0 && w_king == 0 && initialPosition.x == 420 && initialPosition.y == 720 && finalPosition.x == 620 && finalPosition.y == 720)
                {
                    for (int i = 0; i < 32; i++)
                    {
                        if (array_piece[i].temp_y == 720 && (array_piece[i].temp_x == 520 || array_piece[i].temp_x == 620))
                        {
                            isvalidmove = false;
                        }
                    }
                    if (isvalidmove)
                    {
                        array_piece[31].temp_x = 520;
                        PlaySound(castling);
                    }
                }
                else
                {
                    isvalidmove = false;
                }
            }
        }
        ///////////      TIME TO CHECK FOR PHASE    //////////

        if (isvalidmove)
        {
            if (initialPosition.x > finalPosition.x)
            {
                big_x = initialPosition.x;
                diff_x = initialPosition.x - (int)finalPosition.x;
            }
            else
            {
                big_x = (int)finalPosition.x;
                diff_x = (int)finalPosition.x - initialPosition.x;
            }
            if (initialPosition.y > finalPosition.y)
            {
                big_y = initialPosition.y;
                diff_y = initialPosition.y - (int)finalPosition.y;
            }
            else
            {
                big_y = (int)finalPosition.y;
                diff_y = (int)finalPosition.y - initialPosition.y;
            }
            //////////////  KING DOES NOT NEED A PHASE CHECK    //////////////////
            if (index1 == 4 || index1 == 28)
            {
                isvalidmove = true;
            }
            //////////////  ROOK PHASE CHECK    ///////////////////////////////////
            else if (index1 == 0 || index1 == 7 || index1 == 24 || index1 == 31)
            {
                if (initialPosition.x == finalPosition.x)
                {
                    for (int j = 0; j < 32; j++)
                    {
                        for (int i = 1; i < (diff_y / 100); i++)
                        {
                            if ((array_piece[j].temp_y == (big_y - (i * 100))) && (array_piece[j].temp_x == initialPosition.x))
                            {
                                isvalidmove = false;
                                break;
                            }
                        }
                    }
                }
                else if (initialPosition.y == finalPosition.y)
                {
                    for (int j = 0; j < 32; j++)
                    {
                        for (int i = 1; i < (diff_x / 100); i++)
                        {
                            if ((array_piece[j].temp_x == (big_x - (i * 100))) && (array_piece[j].temp_y == initialPosition.y))
                            {
                                isvalidmove = false;
                                break;
                            }
                        }
                    }
                }
                else
                {
                    isvalidmove = false;
                }
            }
            /////////////   BISHOP PHASE CHECK  //////////////////////
            else if (index1 == 2 || index1 == 5 || index1 == 26 || index1 == 29)
            {
                for (int i = 0; i < 32; i++)
                {
                    for (int ii = 1; ii < diff_x / 100; ii++)
                    {
                        /// IF BOTH X AND Y COORDINATES ARE DECREASING
                        if (initialPosition.x > finalPosition.x && initialPosition.y > finalPosition.y)
                        {
                            if ((array_piece[i].temp_x == initialPosition.x - ii * 100) && (array_piece[i].temp_y == initialPosition.y - ii * 100))
                            {
                                isvalidmove = false;
                                break;
                            }
                        }
                        /// IF BOTH X AND Y COORDINATES INCREASING
                        if (initialPosition.x < finalPosition.x && initialPosition.y < finalPosition.y)
                        {
                            if ((array_piece[i].temp_x == finalPosition.x - ii * 100) && (array_piece[i].temp_y == finalPosition.y - ii * 100))
                            {
                                isvalidmove = false;
                                break;
                            }
                        }
                        /// IF X IS INCREASING AND Y IS DECREASING
                        else if (finalPosition.x > initialPosition.x && finalPosition.y < initialPosition.y)
                        {
                            if ((array_piece[i].temp_x == finalPosition.x - ii * 100) && (array_piece[i].temp_y == finalPosition.y + ii * 100))
                            {
                                isvalidmove = false;
                                break;
                            }
                        }
                        /// IF Y IS INCREASING AND X IS DECREASING
                        else if (finalPosition.x < initialPosition.x && finalPosition.y > initialPosition.y)
                        {
                            if ((array_piece[i].temp_x == initialPosition.x - ii * 100) && (array_piece[i].temp_y == initialPosition.y + ii * 100))
                            {
                                isvalidmove = false;
                                break;
                            }
                        }
                    }
                }
            }
            /////////////   KNIGHT DOES NOT NEED A PHASE CHECK  /////////////
            else if (index1 == 1 || index1 == 6 || index1 == 25 || index1 == 30)
            {
                isvalidmove = true;
            }
            ////////////    PAWN PHASE CHECK (FOR BLACK PAWNS)    ////////////////////
            else if ((index1 > 7 && index1 < 16) && initialPosition.y == 120 && finalPosition.y == 320)
            {
                for (int i = 0; i < 32; i++)
                {
                    if (array_piece[i].temp_y == 220 && array_piece[i].temp_x == initialPosition.x)
                    {
                        isvalidmove = false;
                    }
                }
            }
            ////////////    PAWN PHASE CHECK (FOR WHITE PAWNS)    ////////////////////
            else if ((index1 > 15 && index1 < 24) && initialPosition.y == 620 && finalPosition.y == 420)
            {
                for (int i = 0; i < 32; i++)
                {
                    if (array_piece[i].temp_y == 520 && array_piece[i].temp_x == initialPosition.x)
                    {
                        isvalidmove = false;
                    }
                }
            }
            //////////////////////     QUEEN PHASE CHECK   /////////////////////////
            else if (index1 == 3 || index1 == 27)
            {
                if (initialPosition.x == finalPosition.x)
                {
                    for (int j = 0; j < 32; j++)
                    {
                        for (int i = 1; i < (diff_y / 100); i++)
                        {
                            if ((array_piece[j].temp_y == (big_y - (i * 100))) && (array_piece[j].temp_x == initialPosition.x))
                            {
                                isvalidmove = false;
                                break;
                            }
                        }
                    }
                }
                else if (initialPosition.y == finalPosition.y)
                {
                    for (int j = 0; j < 32; j++)
                    {
                        for (int i = 1; i < (diff_x / 100); i++)
                        {
                            if ((array_piece[j].temp_x == (big_x - (i * 100))) && (array_piece[j].temp_y == initialPosition.y))
                            {
                                isvalidmove = false;
                                break;
                            }
                        }
                    }
                }
                else if ((abs(initialPosition.x - finalPosition.x) == abs(initialPosition.y - finalPosition.y)))
                {
                    for (int i = 0; i < 32; i++)
                    {
                        for (int ii = 1; ii < diff_x / 100; ii++)
                        {
                            /// IF BOTH X AND Y COORDINATES ARE DECREASING
                            if (initialPosition.x > finalPosition.x && initialPosition.y > finalPosition.y)
                            {
                                if ((array_piece[i].temp_x == initialPosition.x - ii * 100) && (array_piece[i].temp_y == initialPosition.y - ii * 100))
                                {
                                    isvalidmove = false;
                                    break;
                                }
                            }
                            /// IF BOTH X AND Y COORDINATES INCREASING
                            if (initialPosition.x < finalPosition.x && initialPosition.y < finalPosition.y)
                            {
                                if ((array_piece[i].temp_x == finalPosition.x - ii * 100) && (array_piece[i].temp_y == finalPosition.y - ii * 100))
                                {
                                    isvalidmove = false;
                                    break;
                                }
                            }
                            /// IF X IS INCREASING AND Y IS DECREASING
                            else if (finalPosition.x > initialPosition.x && finalPosition.y < initialPosition.y)
                            {
                                if ((array_piece[i].temp_x == finalPosition.x - ii * 100) && (array_piece[i].temp_y == finalPosition.y + ii * 100))
                                {
                                    isvalidmove = false;
                                    break;
                                }
                            }
                            /// IF Y IS INCREASING AND X IS DECREASING
                            else if (finalPosition.x < initialPosition.x && finalPosition.y > initialPosition.y)
                            {
                                if ((array_piece[i].temp_x == initialPosition.x - ii * 100) && (array_piece[i].temp_y == initialPosition.y + ii * 100))
                                {
                                    isvalidmove = false;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }

        // Draw the texture at the appropriate position

        if (hasInitialPosition && hasFinalPosition && isvalidmove)
        {
            for (int i = 0; i < 32; i++)
            {
                if ((array_piece[i].temp_x == (int)finalPosition.x) && (array_piece[i].temp_y == (int)finalPosition.y))
                {
                    if (i == 4 || i == 28)
                    {
                        PlaySound(checkmate);
                    }
                    if (is_whites_turn && i <= 15)
                    {
                        if (i != 4)
                        {
                            PlaySound(cap_piece);
                        }
                        array_piece[i].temp_x = 1000; // MOVES THE PIECE OUT OF THE BOARD
                        array_piece[i].temp_y = 1000;
                        DrawTexture(array_piece[i].tex, array_piece[i].temp_x, array_piece[i].temp_y, WHITE);
                    }
                    else if (!is_whites_turn && i > 15)
                    {
                        if (i != 28)
                        {
                            PlaySound(cap_piece);
                        }
                        array_piece[i].temp_x = 1000;
                        array_piece[i].temp_y = 1000;
                        DrawTexture(array_piece[i].tex, array_piece[i].temp_x, array_piece[i].temp_y, WHITE);
                    }
                    else
                    {
                        initialPosition = {0, 0};
                        finalPosition = {0, 0};
                        hasInitialPosition = false;
                        hasFinalPosition = false;
                    }
                }
            }

            for (int i = 0; i < 32; i++)
            {
                if ((array_piece[i].temp_x == initialPosition.x) && (array_piece[i].temp_y == initialPosition.y))
                {
                    if (is_whites_turn && i > 15)
                    {
                        array_piece[i].temp_x = (int)finalPosition.x;
                        array_piece[i].temp_y = (int)finalPosition.y;
                        DrawTexture(array_piece[i].tex, array_piece[i].temp_x, array_piece[i].temp_y, WHITE);
                        is_whites_turn = false;
                    }
                    else if (!is_whites_turn && i <= 15)
                    {
                        array_piece[i].temp_x = (int)finalPosition.x;
                        array_piece[i].temp_y = (int)finalPosition.y;
                        DrawTexture(array_piece[i].tex, array_piece[i].temp_x, array_piece[i].temp_y, WHITE);
                        is_whites_turn = true;
                    }
                    else
                    {
                        initialPosition = {0, 0};
                        finalPosition = {0, 0};
                        hasInitialPosition = false;
                        hasFinalPosition = false;
                    }
                }
            }

            initialPosition = {0, 0};
            finalPosition = {0, 0};
            hasInitialPosition = false;
            hasFinalPosition = false;
            index1 = 35;
            index2 = 35;
        }

        pawnpromotioncheck();

        if (gameover() == 0)
        {
            DrawText("WHITE WINS !!!", 140, 320, 80, RED);
        }
        else if (gameover() == 1)
        {
            DrawText("BLACK WINS !!!", 140, 320, 80, RED);
        }

        EndDrawing();
    }

    for (int i = 0; i < 32; i++)
    {
        UnloadTexture(pieces[i]);
    }

    UnloadMusicStream(opening);
    CloseWindow();
    CloseAudioDevice();

    return 0;
}


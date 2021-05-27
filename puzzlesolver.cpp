#include<iostream>
#include<string>
#include<sstream>
#include<fstream>
#include<vector>

using namespace std;

class Tile // Class to store each tile in the puzzle
{
    public:
    int ID;     //ID of a tile default is 0
    int rotn;   //Rotation orientation of a tile
    int edges[4]; // Edge color information

    Tile(int id,int E[],int r); // Constructor
    Tile();                     // Default constructor
    void rotate();              // Method to rotate a tile
    bool isNull();              // Method to check if the tile is NUll(a default value)
};

Tile::Tile()                    // Default constructor definition
{
    ID=0;                       
    rotn=-1;
    edges[0]=0;
    edges[1]=0;
    edges[2]=0;
    edges[3]=0;
}

Tile::Tile(int id,int E[],int r)    //Constructor with argument provided
{
    rotn=r;
    ID=id;
    edges[0]=E[0];
    edges[1]=E[1];
    edges[2]=E[2];
    edges[3]=E[3];

}

bool Tile::isNull()             // IsNULL definition
{
    if (ID==0) return true;
    return false;
}

void Tile::rotate()             // Definition of tile rotation
{
    rotn=(rotn+1)%4;
    int temp = edges[3];
    for(int i=3;i>0;i--)edges[i]=edges[i-1];
    edges[0]=temp;
}

Tile Board[3][3]=               //Main Board
{
    {Tile(),Tile(),Tile()},
    {Tile(),Tile(),Tile()},
    {Tile(),Tile(),Tile()},
};

string pat(int x)       // helper function to decode number to color
{
    string s="";
    if(x==0)s="e0";
    s+='A'+(abs(x)-1);          
    if(x<0)s+="0";
    else s+="1";
    return s;           // returns the decoded string
}

void printPuzzle(stringstream &SS)      // Function to print the board
{
    SS<<"+--------+--------+--------+"<<endl;

    SS<<"|"<<Board[0][0].ID<<"  "<<pat(Board[0][0].edges[0])<<"   |";
    SS<<Board[0][1].ID<<"  "<<pat(Board[0][1].edges[0])<<"   |";
    SS<<Board[0][2].ID<<"  "<<pat(Board[0][2].edges[0])<<"   |"<<endl;

    SS<<"|"<<pat(Board[0][0].edges[3])<<"    "<<pat(Board[0][0].edges[1])<<"|";
    SS<<pat(Board[0][1].edges[3])<<"    "<<pat(Board[0][1].edges[1])<<"|";
    SS<<pat(Board[0][2].edges[3])<<"    "<<pat(Board[0][2].edges[1])<<"|"<<endl;

    SS<<"|   "<<pat(Board[0][0].edges[2])<<"   |";
    SS<<"   "<<pat(Board[0][1].edges[2])<<"   |";
    SS<<"   "<<pat(Board[0][2].edges[2])<<"   |"<<endl;

    SS<<"+--------+--------+--------+"<<endl;

    SS<<"|"<<Board[1][0].ID<<"  "<<pat(Board[1][0].edges[0])<<"   |";
    SS<<Board[1][1].ID<<"  "<<pat(Board[1][1].edges[0])<<"   |";
    SS<<Board[1][2].ID<<"  "<<pat(Board[1][2].edges[0])<<"   |"<<endl;

    SS<<"|"<<pat(Board[1][0].edges[3])<<"    "<<pat(Board[1][0].edges[1])<<"|";
    SS<<pat(Board[1][1].edges[3])<<"    "<<pat(Board[1][1].edges[1])<<"|";
    SS<<pat(Board[1][2].edges[3])<<"    "<<pat(Board[1][2].edges[1])<<"|"<<endl;

    SS<<"|   "<<pat(Board[1][0].edges[2])<<"   |";
    SS<<"   "<<pat(Board[1][1].edges[2])<<"   |";
    SS<<"   "<<pat(Board[1][2].edges[2])<<"   |"<<endl;

    SS<<"+--------+--------+--------+"<<endl;

    SS<<"|"<<Board[2][0].ID<<"  "<<pat(Board[2][0].edges[0])<<"   |";
    SS<<Board[2][1].ID<<"  "<<pat(Board[2][1].edges[0])<<"   |";
    SS<<Board[2][2].ID<<"  "<<pat(Board[2][2].edges[0])<<"   |"<<endl;

    SS<<"|"<<pat(Board[2][0].edges[3])<<"    "<<pat(Board[2][0].edges[1])<<"|";
    SS<<pat(Board[2][1].edges[3])<<"    "<<pat(Board[2][1].edges[1])<<"|";
    SS<<pat(Board[2][2].edges[3])<<"    "<<pat(Board[2][2].edges[1])<<"|"<<endl;

    SS<<"|   "<<pat(Board[2][0].edges[2])<<"   |";
    SS<<"   "<<pat(Board[2][1].edges[2])<<"   |";
    SS<<"   "<<pat(Board[2][2].edges[2])<<"   |"<<endl;

    SS<<"+--------+--------+--------+"<<endl;
}

bool isValid()                  //Function to check whether the board is Valid
{
    for(int i=0;i<2;i++)        //Board is invalid if adjacent edges have different colors
    {
        for(int j=0;j<3;j++)
        {
            if(!Board[i][j].isNull() && !Board[i+1][j].isNull() && (Board[i][j].edges[2] != -Board[i+1][j].edges[0]))
                return false;   
        }
    }
    for(int i=0;i<3;i++)
    {
        for(int j=0;j<2;j++)
        {
            if(!Board[i][j].isNull() && !Board[i][j+1].isNull() && (Board[i][j].edges[1] != -Board[i][j+1].edges[3]))
                return false;
        }
    }
    return true;
}

bool Visited[9];                                        //Array to store whether a tile is visited or not
int allocationOrder[9] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 }; //Array that allocates tiles in the mentioned order
// int allocationOrder[9] = { 4, 3, 5, 1, 7, 0, 2, 6, 8 };  // Unused allocation order which by default avoids rotation symmetries.

vector<string> State;

stringstream Soln;                      //String stream to store the output
int RT[4][9]={                          // Rotation Matrix to get rotation symmetries of the board
    {0,1,2,3,4,5,6,7,8},    
    {6,3,0,7,4,1,8,5,2},
    {8,7,6,5,4,3,2,1,0},
    {2,5,8,1,4,7,0,3,6}
};

string construct(string S,int R[],int x)       // function that encodes a board into a string to avoid repetition 
{                                               // Encoding done according as <tile_number><rotaion_of_tile> from left to right top to bottom
    stringstream ss;

    for(int i=0;i<9;i++)
    {
        ss<<S[RT[x][i]]<<(R[RT[x][i]]+x)%4;
    }
    return ss.str();
}

bool is_prev_soln(string S,int R[9]){       // function taht checks whether a current board already exists in solutions

    string S1 = construct(S,R,0);
    for(int i=0;i<State.size();i++)
    {
        if(State[i]==S1) return true;       //If a solution already exist in the State vectoe 
    }
    
    string S2 = construct(S,R,1);           //Constructing and encoding the symmetries of the board
    string S3 = construct(S,R,2);
    string S4 = construct(S,R,3);

    State.push_back(S1);                    //Storing all encoded rotation symmetry of a valid board int o State vector
    State.push_back(S2);
    State.push_back(S3);
    State.push_back(S4);

    return false;
}

void placenextpiece(int tile_number,vector <Tile> & V,int &count)   //Recursive function that employs 
{                                                                   //backtracking to place each piece in the allocation order                    

    if(tile_number == 9 && isValid() )                              //Case where the complete board is valid
    {
        stringstream ss;
        string S="";
        int R[9];
        for(int i=0;i<3;i++)
        {
            for(int j=0;j<3;j++)
            {
                ss<<Board[i][j].ID;
                R[3*i+j]=Board[i][j].rotn;
            }
        }
        S=ss.str();

        if(!is_prev_soln(S,R)){                     // checking if the current valid board is rotation symmetry of already existing solution 
            count++;
            printPuzzle(Soln);
        }
    
        return;
    }

    else                                                            // Placing a piece on the board
    {
        int nextpos = allocationOrder[tile_number];

        // int maxR = (tile_number == 4) ? 1 : 4;    
        int maxR = 4;                   // No of rotations allowed

        for(int pieceID = 0; pieceID<9; pieceID++)
        {
            if(Visited[pieceID]==false)
            {
                Visited[pieceID] = true;                    //Marking the current tile as visited

                for(int rotate=0; rotate<maxR;rotate++)     // trying all rotative permutations of current tile
                {
                    Board[(nextpos/3)][(nextpos%3)] = Tile(V[pieceID].ID,V[pieceID].edges,V[pieceID].rotn); // placing a tile on the board
                    if(isValid())   
                    {
                        placenextpiece(tile_number+1,V,count);      // recursive call to place the next tile
                    }
                    V[pieceID].rotate();
                }
                Visited[pieceID] = false;               //Bactracking the visited tile

                Board[nextpos/3][nextpos%3] = Tile();   // Backtracking the board to a NULL/default tile

            }
        }
    }
}

int main(int argc, char *argv[])
{
    if(argc<2){
        cout<<"No text file provided"<<endl;
    }

    ifstream fptr ;         
    fptr.open(argv[1]);             // opening the input file

    vector<Tile> Puzzle;            //vectore to store a tile
    
    for(int i=0;i<9;i++)
    {
        string input_line;
        getline(fptr,input_line,'\n');  // reading a line from the file

        stringstream S(input_line);
        string tok;

        int E[4],j=0;

        while(getline(S,tok,','))       // tokenizing the the line, extracting comma separated values
        {
            E[j]=tok[0]-'A' + 1;
            if(tok[1]=='0')E[j]*=-1;        //Encodes the color as a positive or negative integer eg. A0 = -1, A1 = 1, C0 = -3 and similarly
            j++;
        }
        
        Puzzle.push_back(Tile(i+1,E,0));      // Storing a tile
    }
    fptr.close();
    
    cout<<"Input tiles:"<<endl;
    
    for(int i=0;i<9;i++)        // Printing the input 
    {
        cout<<i+1<<". <"<<pat(Puzzle[i].edges[0])<<", "<<pat(Puzzle[i].edges[1])<<", "<<pat(Puzzle[i].edges[2])<<", "<<pat(Puzzle[i].edges[3])<<">"<<endl;
    }
    cout<<endl;

    int count = 0;
    for(int i=0;i<9;i++)Visited[i]=false;
    
    placenextpiece(0,Puzzle,count); // calling the recursive function to solve the puzzle

    //Printing the number of solutions
    if(count==0) cout <<"No solution found."<<endl;
    else if (count ==1) cout<<"1 unique solution found"<<endl;
    else cout<<count<<" unique solutions found:"<<endl;

    cout<<Soln.str();   // Printing the board if a solution exists

    return 1;
}
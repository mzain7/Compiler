// Muhammad Zain
// FA21-BCS-144

#include <iostream>
#include <fstream>
#include <cctype>
#include <string>

using namespace std;

#define BUFFER_SIZE 4096

struct BufferNode
{
    char buffer[BUFFER_SIZE];
    int size;
    int pos;
    BufferNode *next;

    BufferNode()
    {
        size = 0;
        pos = 0;
        next = nullptr;
    }
};

void generateToken(const string &lexeme, const string &type)
{
    cout << "Token: " << lexeme << ", Token Type: " << type << endl;
}

bool loadBuffer(ifstream &file, BufferNode *&bufferHead, BufferNode *&tail)
{
    BufferNode *newNode = new BufferNode();
    file.read(newNode->buffer, BUFFER_SIZE);
    newNode->size = file.gcount();
    if (newNode->size == 0)
    {
        delete newNode;
        return false;
    }
    if (!bufferHead)
    {
        bufferHead = newNode;
    }
    if (tail)
    {
        tail->next = newNode;
    }
    tail = newNode;
    return true;
}

bool getChar(char &c, BufferNode *&currentBuffer, ifstream &file)
{
    if (!currentBuffer || currentBuffer->pos >= currentBuffer->size)
    {
        if (!loadBuffer(file, currentBuffer, currentBuffer))
        {
            return false;
        }
        currentBuffer->pos = 0;
    }
    c = currentBuffer->buffer[currentBuffer->pos++];
    return true;
}

bool ungetChar(BufferNode *&currentBuffer)
{
    if (currentBuffer->pos > 0)
    {
        currentBuffer->pos--;
        return true;
    }
    return false;
}

int main()
{
    ifstream file("file.txt", ios::binary);
    if (!file.is_open())
    {
        cerr << "File not found" << endl;
        return 1;
    }

    BufferNode *bufferHead = nullptr, *bufferTail = nullptr, *currentBuffer = nullptr;
    int state = 0;
    char c;
    string lexeme;

    while (true)
    {
        if (!getChar(c, currentBuffer, file))
        {
            break;
        }

        switch (state)
        {
        case 0:
            if (c == '/')
            {
                char nextChar;
                if (getChar(nextChar, currentBuffer, file))
                {
                    if (nextChar == '/')
                    {
                        state = 3;
                        lexeme += "//";
                    }
                    else if (nextChar == '*')
                    {
                        state = 4;
                        lexeme += "/*";
                    }
                    else
                    {
                        lexeme += c;
                        ungetChar(currentBuffer);
                    }
                }
            }
            break;

        case 3:
            while (c != '\n' && getChar(c, currentBuffer, file))
            {
                lexeme += c;
            }
            generateToken(lexeme, "Single-line Comment");
            lexeme.clear();
            state = 0;
            break;

        case 4:
            while (true)
            {
                if (!getChar(c, currentBuffer, file))
                {
                    break;
                }
                lexeme += c;
                if (c == '*')
                {
                    char nextChar;
                    if (getChar(nextChar, currentBuffer, file))
                    {
                        if (nextChar == '/')
                        {
                            lexeme += nextChar;
                            generateToken(lexeme, "Multi-line Comment");
                            lexeme.clear();
                            state = 0;
                            break;
                        }
                        else
                        {
                            ungetChar(currentBuffer);
                        }
                    }
                }
            }
            break;
        }
    }

    if (!lexeme.empty())
    {
        if (state == 3)
        {
            generateToken(lexeme, "Single-line Comment");
        }
        else if (state == 4)
        {
            generateToken(lexeme, "Multi-line Comment");
        }
    }

    while (bufferHead)
    {
        BufferNode *temp = bufferHead;
        bufferHead = bufferHead->next;
        delete temp;
    }

    file.close();
    return 0;
}

/*
The dynamic buffer loads data from a file in 4096-byte chunks using a linked list of `BufferNode` structures, allowing for efficient memory management.
The state machine processes characters through defined states:
starting from an initial state,it transitions to identifier or comment states based on the characters read.
Single-line comments are detected until a newline, while multi-line comments are detected until the `*\/` sequence is found.
*/
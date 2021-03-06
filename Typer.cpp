/* Author: Brendon Kofink
 *         Johann Rajadurai
 *         Aaron Sierra
 *         David Day
 *         Lucy Ray
 * Assignment Title: Ball Game
 * Assignment Description: user can launch balls to hit objects.
 * Due Date: 12/08/2021
 * Date Created: 11/30/2021
 * Date Last Modified: 12/03/2021
 */

#include "Typer.h"

//Constructor
TextObject::TextObject(string text, vec2 pos, int size, Color color, bool back, bool del)
{
  this->text = text;
  this->pos = pos;
  this->size = size;
  this->layer = layer;
  this->color = color;
  this->back = back;
  this->del = del;
}

Typer::Typer()
{
  ImportLetters();
  ImportNumbers();
  ImportSymbols();
}

//Each of these imports does the same thing.
//Just loop through the proper file and add to array.

void Typer::ImportLetters()
{
  ifstream file("A.txt");
  char c;
  for (int i = 0; i < 26; i++)
  {
    for (int j = 0; j < 5; j++)
    {
      for (int k = 0; k < 5; k++)
      {
        file.get(c);
        if (c == '1')
        {
          characters[i][j][k] = true;
        }
        else
        {
          characters[i][j][k] = false;
        }
      }
      file.ignore();
    }
  }

  file.close();
}

void Typer::ImportNumbers()
{
  ifstream file("num.txt");
  char c;
  for (int i = 0; i < 10; i++)
  {
    for (int j = 0; j < 5; j++)
    {
      for (int k = 0; k < 5; k++)
      {
        file.get(c);
        if (c == '1')
        {
          characters[i + 26][j][k] = true;
        }
        else
        {
          characters[i + 26][j][k] = false;
        }
      }
      file.ignore();
    }
  }

  file.close();
}

void Typer::ImportSymbols()
{
  ifstream file("special.txt");
  char c;
  for (int i = 0; i < 5; i++)
  {
    for (int j = 0; j < 5; j++)
    {
      for (int k = 0; k < 5; k++)
      {
        file.get(c);
        if (c == '1')
        {
          characters[i + 36][j][k] = true;
        }
        else
        {
          characters[i + 36][j][k] = false;
        }
      }
      file.ignore();
    }
  }

  file.close();
}

bool Typer::TryLetter(char c, int &index)
{
  bool found = false;
  if (c == '!')
  {
    index = 36;
    found = true;
  }
  else if (c == ':')
  {
    index = 37;
    found = true;
  }
  else if (c == '.')
  {
    index = 38;
    found = true;
  }
  else if (c == '-')
  {
    index = 39;
    found = true;
  }
  else if (c == '+')
  {
    index = 40;
    found = true;
  }
  return found;
}

//Helper function
void Typer::DrawBox(SDL_Plotter &g, int x, int y, int size, Color c)
{
  for (int i = 0; i < size; i++)
  {
    for (int j = 0; j < size; j++)
    {
      if (x+i >= 0 && x+i < g.getCol() && y+j >= 0 && y+j < g.getRow())
      {
        g.plotPixel(x + i, y + j, c.R, c.G, c.B);
      }
    }
  }
}

void Typer::Write(string word, SDL_Plotter &g, vec2 pos, Color c, int size, bool back)
{
  int character;
  for (int i = 0; i < word.length(); i++)
  {
    //Does different things depending what the character is.
    if (isalpha(word[i]))
    {
      character = toupper(word[i]) - 'A';
    }
    else if (isdigit(word[i]))
    {
      character = word[i] - '0' + 26;
    }
    else if (!TryLetter(word[i], character))
    {
      character = 0;
    }
    for (int j = 0; j < 5; j++)
    {
      for (int k = 0; k < 5; k++)
      {
        if (characters[character][j][k] && word[i] != ' ')
        {
          //Draw pixel at pos + i, 5 is size of character * size of font.
          DrawBox(g, pos.x + i * 5 * size + i * size + k * size, pos.y + j * size, size, c);
        }
        else if (back)
        {
          DrawBox(g, pos.x + i * 5 * size + i * size + k * size, pos.y + j * size, size, BLANK);
        }
      }
    }
  }
}

//Writes TextObject instead of normal parameters. 
void Typer::Write(TextObject text, SDL_Plotter &g)
{
  Write(text.text, g, text.pos, text.color, text.size, text.back);
}

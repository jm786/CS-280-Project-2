#include <iostream>
#include <algorithm>
#include <cstring>
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include "tokens.h"

string getTokenName(TokenType ttype){
  switch(ttype){
    case PRINT: return "PRINT";
    case IF: return "IF";
    case THEN: return "THEN";
    case TRUE: return "TRUE";
    case FALSE: return "FALSE";
    case IDENT: return "IDENT";
    case ICONST: return "ICONST";
    case SCONST: return "SCONST";
    case PLUS: return "PLUS";
    case MINUS: return "MINUS";
    case STAR: return "STAR";
    case SLASH: return "SLASH";
    case ASSIGN: return "ASSIGN";
    case EQ: return "EQ";
    case NEQ: return "NEQ";
    case LT: return "LT";
    case LEQ: return "LEQ";
    case GT: return "GT";
    case GEQ: return "GEQ";
    case LOGICAND: return "LOGICAND";
    case LOGICOR: return "LOGICOR";
    case LPAREN: return "LPAREN";
    case RPAREN: return "RPAREN";
    case SC: return "SC";
    case ERR: return "ERR";
    case DONE: return "DONE";
  }

  return "DONE";
}

TokenType keywordchecker(const string& word){

  if (word == "print"){
    return PRINT;
  }

  else if (word == "if"){
    return IF;
  }

  else if (word == "then"){
    return THEN;
  }

  else if (word == "true"){
    return TRUE;
  }

  else if (word == "false"){
    return FALSE;
  }

  return IDENT;

}

Token getNextToken(istream *in, int *linenumber){

    enum LexState {BEGIN, INID, ININT, INSTRING, INCOMMENT};
    LexState lexstate = BEGIN;
    string lexeme = "";
    char ch;
    char nextCharacter;
    TokenType returnType = ERR;
    Token returnToken;

    while (in -> get(ch)){
      if (ch == EOF)
        return Token(DONE, lexeme, *linenumber);

      else if (ch == '\n')
        (*linenumber)++;

      switch (lexstate){
        case BEGIN:
          if (std::isspace(ch)){
            continue;
          }

          lexeme += ch;

          if (ch == '+'){
            return Token(PLUS, lexeme, *linenumber);
          }

          else if (ch == '-'){
            return Token(MINUS, lexeme, *linenumber);
          }

          else if (ch == '*'){
            return Token(STAR, lexeme, *linenumber);
          }

          else if (ch == '/'){
            return Token(SLASH, lexeme, *linenumber);
          }

          else if (ch == '('){
            return Token(LPAREN, lexeme, *linenumber);
          }

          else if (ch == ')'){
            return Token(RPAREN, lexeme, *linenumber);
          }

          else if (ch == ';'){
            return Token(SC, lexeme, *linenumber);
          }

          else if (ch == '='){
            in -> get(nextCharacter);
            if (nextCharacter == '=')
              return Token(EQ, lexeme, *linenumber);

            else{
              return Token(ASSIGN, lexeme, *linenumber);
            }
          }

          else if (ch == '!'){
            in -> get(nextCharacter);
            if (nextCharacter == '=')
              return Token(NEQ, lexeme, *linenumber);

            else{
              return Token(ERR, lexeme, *linenumber);
            }
          }

          else if (ch == '>'){
            in -> get(nextCharacter);
            if (nextCharacter == '=')
              return Token(GEQ, lexeme, *linenumber);

            else
              return Token(GT, lexeme, *linenumber);
          }

          else if (ch == '<'){
            in -> get(nextCharacter);
            if (nextCharacter == '=')
              return Token(LEQ, lexeme, *linenumber);

            else
              return Token(LT, lexeme, *linenumber);
          }

          else if (ch == '&'){
            in -> get(nextCharacter);
            if (nextCharacter == '&')
              return Token(LOGICAND, lexeme, *linenumber);

            else
              return Token(ERR, lexeme, *linenumber);
          }

          else if (ch == '|'){
            in -> get(nextCharacter);
            if (nextCharacter == '|')
              return Token(LOGICOR, lexeme, *linenumber);

            else
              return Token(ERR, lexeme, *linenumber);
          }

          else if (ch == '#'){
            lexstate = INCOMMENT;
          }

          else if (std::isdigit(ch)){
            lexstate = ININT;
          }

          else if (std::isalpha(ch)){
            lexstate = INID;
          }

          else if (ch == '"'){
            lexstate = INSTRING;
          }

          else{
            return Token(DONE, lexeme, *linenumber);
          }

          break;

        case INID:
          if (std::isalpha(ch)){
            lexeme += ch;
            continue;
          }

          in -> putback(ch);
          
          return Token(keywordchecker(lexeme), lexeme, *linenumber);

        case ININT:
          if (std::isdigit(ch)){
            lexeme += ch;
            continue;
          }

          in ->putback(ch);
          (*linenumber)--;

          return Token(ICONST, lexeme, *linenumber);

        case INSTRING:
          lexeme += ch;

          if (ch == '\n'){
            in -> putback(ch);
            (*linenumber)--;
            return Token(ERR, lexeme, *linenumber);
          }

          else if (ch != '"'){
            continue;
          }

          return Token(SCONST, lexeme, *linenumber);

        case INCOMMENT:
          if (ch != '\n'){
            continue;
          }

          lexeme.clear();
          lexstate = BEGIN;
          break;

        default:
          //This will never happen
          std::cerr << "Error that should not happen" << std::endl;
          exit(1);
          break;
      }
    }

    if (lexstate == INID){
      return Token(keywordchecker(lexeme), lexeme, *linenumber);
    }
    else if (lexstate == ININT){
      return Token(ICONST, lexeme, *linenumber);
    }
    else {
      return Token(DONE, lexeme, *linenumber);
    }

    returnToken = Token(returnType, lexeme, *linenumber);

    return returnToken;
}


int main(int argc, char *argv[]){

    std::filebuf file;
    int filenum = 1, linenumber = 1;
    int stats[3] = {0};
    bool cases[3] = {false};

    for (int i = 1; i < argc; i++){

        std::string arg(argv[i]);

        if (*argv[i]++ == '-'){
            char a = *argv[i]++;

            switch (a){

                case ('v'):
                    if (strcmp(argv[i], "\0") == 0){
                      cases[0] = true;
                      break;
                    }
                case ('s'):
                    if (strcmp(argv[i], "um") == 0){
                      cases[1] = true;
                      break;
                    }
                case ('a'):
                    if (strcmp(argv[i], "llids") == 0){
                      cases[2] = true;
                      break;
                    }
                default:
                    std::cout << "INVALID FLAG " << arg << std::endl;
                    return(0);
            }
        }

        else if ((i == (argc-1)) && (filenum == 1)){
            filenum++;
            file.open(arg, std::ios::in);
            std::istream infile(&file);

            if (!file.is_open()){
                std::cout << "UNABLE TO OPEN " << arg << std::endl;
                return(0);
            }
            else if (infile.eof())
                return(0);

        }

        else {
            std::cout << "TOO MANY FILE NAMES" << std::endl;
            return(0);
        }
    }

    std::istream infile(&file);

    Token myToken;

    while ((myToken = getNextToken(&infile, &linenumber)) != DONE && myToken != ERR){

      stats[2]++;

      if (myToken.GetTokenType() == SCONST)
        stats[0]++;
      else if (myToken.GetTokenType() == IDENT)
        stats[1]++;


      if (cases[0] && ((myToken.GetTokenType() == IDENT) || (myToken.GetTokenType() == ICONST) || (myToken.GetTokenType() == SCONST))){
        string text = myToken.GetLexeme();
        text.erase(remove(text.begin(), text.end(), '\"'), text.end());

        std::cout << getTokenName(myToken.GetTokenType()) << "(" << text << ")" << std::endl;
      }
      else if (cases[0]){
          std::cout << getTokenName(myToken.GetTokenType()) << std::endl;
      }
    }

    if (myToken.GetTokenType() == ERR){
      string text = myToken.GetLexeme();
      std::cout << "Error on line " << myToken.GetLinenum() << " (" << text << ")" << std::endl;
    }

    if (cases[1]){
      std::cout << "Total lines: " << linenumber << std::endl;
      std::cout << "Total tokens: " << stats[2] << std::endl;
      std::cout << "Total identifiers: " << stats[1] << std::endl;
      std::cout << "Total strings: " << stats[0] << std::endl;
    }

    (void)filenum;    //(SOLUTION) unused error returned since it's only used in the if statements

    return 0;
}

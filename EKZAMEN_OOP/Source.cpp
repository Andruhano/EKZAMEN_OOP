#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>

using namespace std;

class WordBank
{
vector<string> words;
string decryptWord(const string& encryptedWord)
{
    string decryptedWord = encryptedWord;
    for (char& c : decryptedWord)
    {
        if (isalpha(c))
        {
            c = ((tolower(c) - 'a' + 13) % 26) + 'a';
        }
    }
    return decryptedWord;
}

public:
    void loadWords(const string& filename)
    {
        ifstream file("wordlist.txt");
        if (file.is_open())
        {
            string word;
            while (getline(file, word))
            {
                words.push_back(decryptWord(word));
            }
            file.close();
        }
        else
        {
            cout << "Unable to open file: " << "wordlist.txt" << endl;
        }
    }

    string getRandomWord()
    {
        if (!words.empty())
        {
            int randomIndex = rand() % words.size();
            return words[randomIndex];
        }
        else
        {
            cout << "Word bank is empty!" << endl;
            return "";
        }
    }
};

class Game 
{
string secretWord;
string guessedLetters;
int attemptsLeft;
bool gameWon;
chrono::steady_clock::time_point startTime;

public:
    Game() : attemptsLeft(6), gameWon(false) {}

    void startGame(const string& filename) 
    {
        WordBank wordBank;
        wordBank.loadWords("wordlist.txt");
        secretWord = wordBank.getRandomWord();
        guessedLetters = string(secretWord.length(), '_');
        attemptsLeft = 6;
        gameWon = false;
        startTime = chrono::steady_clock::now();
    }

    void guessLetter(char letter) 
    {
        bool letterGuessed = false;
        letter = tolower(letter); 
        for (int i = 0; i < secretWord.length(); i++) 
        {
            if (secretWord[i] == letter) 
            {
                guessedLetters[i] = letter;
                letterGuessed = true;
            }
        }
        if (!letterGuessed) 
        {
            attemptsLeft--;
        }
        if (guessedLetters == secretWord) 
        {
            gameWon = true;
        }
    }

    bool isGameOver() 
    {
        return attemptsLeft <= 0 || gameWon;
    }

    void printStats() 
    {
        cout << "Guessed letters: " << guessedLetters << endl;
        cout << "Attempts left: " << attemptsLeft << endl;
        if (gameWon) 
        {
            auto endTime = chrono::steady_clock::now();
            auto duration = chrono::duration_cast<chrono::seconds>(endTime - startTime);
            cout << "Congratulations! You won in " << duration.count() << " seconds!" << endl;
        }
        else if (attemptsLeft <= 0) 
        {
            cout << "Game over! You lost!" << endl;
            cout << "The word was: " << secretWord << endl;
            cout << "    +---+" << endl;
            cout << "    |   |" << endl;
            cout << "    O   |" << endl;
            cout << "   /|\\  |" << endl;
            cout << "   / \\  |" << endl;
            cout << "        |" << endl;
            cout << "===========" << endl;
        }
        else 
        {
            cout << "    +---+" << endl;
            cout << "    |   |" << endl;
            if (attemptsLeft < 6) 
            {
                cout << "    O   |" << endl;
            }
            else 
            {
                cout << "        |" << endl;
            }
            if (attemptsLeft < 5) 
            {
                cout << "   /|\\  |" << endl;
            }
            else 
            {
                cout << "        |" << endl;
            }
            if (attemptsLeft < 4) 
            {
                cout << "   / \\  |" << endl;
            }
            else 
            {
                cout << "        |" << endl;
            }
            cout << "        |" << endl;
            cout << "===========" << endl;
        }
    }
};

int main() 
{
    srand(time(nullptr));
    WordBank wordBank;
    const string filename = "wordlist.txt";

    Game game;
    game.startGame(filename);

    while (!game.isGameOver()) 
    {
        game.printStats();
        cout << "Enter a letter: ";
        char letter;
        cin >> letter;
        game.guessLetter(letter);
    }

    game.printStats();

    return 0;
}
#include <SDL2/SDL.h>

using std::unordered_set;

bool operator==(SDL_Color a, SDL_Color b);

bool operator!=(SDL_Color a, SDL_Color b);

unordered_set<int> &operator-=(unordered_set<int> &a, unordered_set<int> &b);

unordered_set<int> &operator^=(unordered_set<int> &a, unordered_set<int> &b);

bool operator==(unordered_set<int> &a, unordered_set<int> &b);

bool operator!=(unordered_set<int> &a, unordered_set<int> &b);

bool operator>(unordered_set<int> &a, unordered_set<int> &b);

bool operator<(unordered_set<int> &a, unordered_set<int> &b);

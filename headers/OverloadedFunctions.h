bool operator==(SDL_Color a, SDL_Color b)
{
  if ( (a.r == b.r) && (a.g == b.g) && (a.b == b.b) && (a.a == b.a) ) 
    return true;
  else
    return false;
}

bool operator!=(SDL_Color a, SDL_Color b)
{
  if ( (a.r == b.r) && (a.g == b.g) && (a.b == b.b) && (a.a == b.a) ) 
    return false;
  else
    return true;
}

unordered_set<int> &operator-=(unordered_set<int> &a, unordered_set<int> &b)
{
  for (auto i = b.begin(); i != b.end(); i++ )
  {
    if (a.count(*i) )
      a.erase(*i);
  }
  return a;
}

bool operator==(unordered_set<int> &a, unordered_set<int> &b)
{
  for (auto i = b.begin(); i != b.end(); i++ )
  {
    if (!a.count(*i))
      return false;
  }

  for (auto i = a.begin(); i != a.end(); i++ )
  {
    if (!b.count(*i))
      return false;
  }
  return true;
}

bool operator!=(unordered_set<int> &a, unordered_set<int> &b)
{
  for (auto i = b.begin(); i != b.end(); i++ )
  {
    if (!a.count(*i))
      return true;
  }

  for (auto i = a.begin(); i != a.end(); i++ )
  {
    if (!b.count(*i))
      return true;
  }
  return false;
}

bool operator>(unordered_set<int> &a, unordered_set<int> &b)
{
  bool b_in_a = true;
  for (auto i = b.begin(); i != b.end(); i++ )
  {
    if (!a.count(*i))
    {  
      return b_in_a = false;
      return false;
    }
  }

  bool a_not_in_b = false;
  for (auto i = a.begin(); i != a.end(); i++ )
  {
    if (!b.count(*i))
      a_not_in_b = true;
  }

  return (b_in_a && a_not_in_b);
}

bool operator<(unordered_set<int> &a, unordered_set<int> &b)
{
  bool a_in_b = true;
  for (auto i = a.begin(); i != a.end(); i++ )
  {
    if (!b.count(*i))
    {  
      return a_in_b = false;
      return false;
    }
  }

  bool b_not_in_a = false;
  for (auto i = b.begin(); i != b.end(); i++ )
  {
    if (!a.count(*i))
      b_not_in_a = true;
  }

  return (a_in_b && b_not_in_a);
}

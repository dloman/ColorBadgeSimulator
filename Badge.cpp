#include "Badge.hpp"
#include <random>

using ct::Badge;

std::mt19937 Generator(std::random_device{}());
std::uniform_int_distribution<unsigned char> RandomColor(0, 255);
std::uniform_int_distribution<> RandomPosition(10, ct::Size + 10);

std::uniform_int_distribution<unsigned char> ColorUpdate(1, 3);
std::uniform_int_distribution<> PositionUpdate(-1, 1);
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
Badge::Badge()
: mColor(
    RandomColor(Generator),
    RandomColor(Generator),
    RandomColor(Generator),
    RandomColor(Generator)),
  mAlphaIncrease(true),
  mPostion(RandomPosition(Generator), RandomPosition(Generator)),
  mLastUpdateTime(0)
{
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
const wxPoint Badge::GetPosition() const
{
  std::lock_guard Lock(mMutex);

  return mPostion;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
const wxColor Badge::GetColor() const
{
  std::lock_guard Lock(mMutex);

  return mColor;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void Badge::Update()
{
  std::lock_guard Lock(mMutex);

  Fade();

  mPostion.x += PositionUpdate(Generator);

  mPostion.y += PositionUpdate(Generator);
}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void Badge::Fade()
{
  if (mAlphaIncrease)
  {
    if (mColor.Alpha() >= 254u)
    {
      mAlphaIncrease = false;
    }

    mColor.Set(
      mColor.Red(),
      mColor.Green(),
      mColor.Blue(),
      mColor.Alpha());

  }
  else
  {
    if (mColor.Alpha() <= 1u)
    {
      mAlphaIncrease = true;
    }

    mColor.Set(
      mColor.Red(),
      mColor.Green(),
      mColor.Blue(),
      mColor.Alpha());
  }
}

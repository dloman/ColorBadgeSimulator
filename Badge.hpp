#pragma once

#include <wx/colour.h>
#include <wx/gdicmn.h>
#include <chrono>
#include <mutex>

namespace ct
{
  constexpr size_t Size = 1000u;

  class Badge
  {
    public:

      Badge();

      const wxPoint GetPosition() const;

      const wxColor GetColor() const;

      void Update();

    private:

      void Fade();

    private:

      wxColor mColor;

      bool mAlphaIncrease;

      wxPoint mPostion;

      std::chrono::milliseconds mLastUpdateTime;

      mutable std::mutex mMutex;
  };
}



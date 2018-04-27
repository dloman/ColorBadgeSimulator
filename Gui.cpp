#include <wx/wx.h>
#include <wx/graphics.h>
#include <atomic>
#include <chrono>
#include <vector>
#include <thread>
#include "Badge.hpp"

constexpr int BadgeSize = 10;
constexpr size_t NumberOfBadges = 100u;

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
class Window : public wxWindow
{
  public:

    Window(wxWindow*, wxWindowID, const std::vector<ct::Badge>&);

  private:

    void OnPaint(wxPaintEvent& Event);

  private:

    const std::vector<ct::Badge>& mBadges;
};

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
Window::Window(
  wxWindow* pParent,
  wxWindowID Id,
  const std::vector<ct::Badge>& Badges)
  : wxWindow(pParent, Id),
    mBadges(Badges)
{
  Bind(wxEVT_PAINT, [this](wxPaintEvent& Event)
  {
    OnPaint(Event);
  });
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void Window::OnPaint(wxPaintEvent& Event)
{
  wxPaintDC Dc(this);

  Dc.SetBackground(*wxBLACK_BRUSH);

  Dc.Clear();

  auto pGraphicsContext = wxGraphicsContext::Create(Dc);

  if (pGraphicsContext)
  {
    auto Path = pGraphicsContext->CreatePath();

    for (const auto& Badge : mBadges)
    {
      const auto Color = Badge.GetColor();

      pGraphicsContext->SetBrush(wxBrush(Color));

      const auto Position = Badge.GetPosition();

      pGraphicsContext->DrawEllipse(Position.x, Position.y, BadgeSize, BadgeSize);
    }
  }
}

//******************************************************************************
//******************************************************************************
class App : public wxApp
{
  public:

    App();

    ~App();

    bool OnInit() override;

  private:

    std::vector<ct::Badge> mBadges;

    std::unique_ptr<std::thread> mpThread;

    std::atomic<bool> mIsRunning;
};

IMPLEMENT_APP(App);

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
App::App()
: mBadges(NumberOfBadges),
  mpThread(nullptr),
  mIsRunning(true)
{
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
App::~App()
{
  mIsRunning = false;

  mpThread->join();
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
bool App::OnInit()
{
  auto pFrame =
    new wxFrame(
      nullptr,
      wxID_ANY,
      wxT("Color Thingy"),
      wxPoint(200, 200),
      wxSize(ct::Size + 50, ct::Size + 50));

  wxBoxSizer* pSizer = new wxBoxSizer(wxHORIZONTAL);

  auto pWindow = new Window(pFrame, wxID_ANY, mBadges);

  pSizer->Add(pWindow, 1, wxEXPAND);

  pFrame->SetSizer(pSizer);

  pFrame->Show();

  mpThread = std::make_unique<std::thread>( [this, pWindow]
    {
      while (mIsRunning)
      {
        for (auto& Badge : mBadges)
        {
          Badge.Update();
        }

        pWindow->Refresh();

        std::this_thread::sleep_for(std::chrono::milliseconds(5));
      }
    });

  return true;
}

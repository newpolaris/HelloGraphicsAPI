#pragma once

#include <GLFW/glfw3.h>
#include <graphics_types.h>
#include <string>

struct GLFWwindow;

namespace el {

    struct GraphicsWindowDesc;

    typedef std::shared_ptr<class GraphicsWindow> GraphicsWindowPtr;
    typedef std::weak_ptr<class GraphicsWindow> GraphicsWindowWeakPtr;

    enum GraphicsWindowType
    {
        GraphicsWindowTypeNone = 0,
        GraphicsWindowTypeGLFW = 1,
    };

    GraphicsWindowPtr createWindow(const GraphicsWindowDesc& desc);

    struct GraphicsWindowDesc
    {
    public:

        void setDeviceType(GraphicsDeviceType type);
        GraphicsDeviceType getDeviceType() const;

        void setWindowType(GraphicsWindowType type);
        GraphicsWindowType getWindowType() const;

        void setWindowTitle(const std::string& title);
        const std::string& getWindowTitle() const;

        void setWidth(uint32_t width);
        uint32_t getWidth() const;

        void setHeight(uint32_t height);
        uint32_t getHeight() const;

    private:

        uint32_t _width;
        uint32_t _height;
        std::string _windowTitle;
        GraphicsDeviceType _deviceType;
        GraphicsWindowType _windowType;
    };

    class GraphicsWindow 
    {
    public:

        GraphicsWindow();
        virtual ~GraphicsWindow();

        virtual bool run() = 0;
    };

    class GraphicsWindowGLFW final : public GraphicsWindow
    {
    public:

        GraphicsWindowGLFW();
        ~GraphicsWindowGLFW();

        bool setup(const GraphicsWindowDesc& desc);
        void close(); 

        bool run() override;

    // private:

        static void errorCallback(int error, const char* description);
        static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

        bool setupGLFW();
        bool setupWindow();
        
        GLFWwindow* _window;
        GraphicsWindowDesc _windowDesc;
    };

} // namespace el;


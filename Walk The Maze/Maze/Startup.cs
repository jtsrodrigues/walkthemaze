using Microsoft.Owin;
using Owin;

[assembly: OwinStartupAttribute(typeof(Maze.Startup))]
namespace Maze
{
    public partial class Startup
    {
        public void Configuration(IAppBuilder app)
        {
            ConfigureAuth(app);
        }
    }
}

using Microsoft.Owin;
using Owin;

[assembly: OwinStartupAttribute(typeof(WebDisPlay.Startup))]
namespace WebDisPlay
{
    public partial class Startup
    {
        public void Configuration(IAppBuilder app)
        {
            ConfigureAuth(app);
        }
    }
}

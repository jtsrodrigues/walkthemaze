using Maze.Models;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.Serialization;
using System.ServiceModel;
using System.Text;
using System.Web;
using System.Web.Security;


using Microsoft.AspNet.Identity;
using Microsoft.AspNet.Identity.EntityFramework;


namespace Maze
{
    // NOTE: You can use the "Rename" command on the "Refactor" menu to change the class name "ServiceSF" in code, svc and config file together.
    // NOTE: In order to launch WCF Test Client for testing this service, please select ServiceSF.svc or ServiceSF.svc.cs at the Solution Explorer and start debugging.
    public class ServiceSF : IServiceSF
    {
        public bool InsertUploadData(string user, int score, string mapa)
        {
            using (ApplicationDbContext db = new ApplicationDbContext())
            {

                var userValidation = db.Users.Where(u => u.UserName == user);
                if (userValidation.Count() > 0)
                {
                    //var username = db.Leaderboard.Where(s => s.User == user).SingleOrDefault();
                    //if (username != null)
                    //{
                    //    username.Score = score;
                    //    db.SaveChanges();
                    //}
                    //else
                    //{
                    var mapaValidation = db.Mapa.Where(m => m.Nome == mapa);
                    MapaModel map = db.Mapa.Where(m => m.Nome== mapa).Single();
                    if (mapaValidation.Count() > 0)
                    {
                        var us = new LeaderboardModel();
                        us.ScoreDate = DateTime.Now;
                        us.User = user;
                        us.Score = score;
                        us.MundoID = map.ID;
                        db.Leaderboard.Add(us);

                        db.SaveChanges();

                        return true;
                    }
                }
                return false;
            }
        }

        public bool CheckLogin(string user, string password)
        {
            //using (UsersContext db = new UsersContext())
            //{
            //var username = db.UserProfiles.Where(u => u.UserName == user).SingleOrDefault();
            //    if (username == null) return false;

            //var provider = Membership.Provider;
            //string name = provider.ApplicationName;

            //using (var context = new UsersContext())
            //    context.UserProfiles.Find(1);

            //if (!WebSecurity.Initialized)
            //    WebSecurity.InitializeDatabaseConnection("DefaultConnection", "UserProfile", "UserId", "UserName", autoCreateTables: true);
            //var val = ValidateUser(user, password);
            ////var val = WebSecurity.Login(username.UserName, password);
            ////var vali = FormsAuthentication.Authenticate(user, password);
            //if (val)
            //    return true;
            //return false;
            
            var um = new UserManager<ApplicationUser>(new UserStore<ApplicationUser>(new ApplicationDbContext()));
            try
            {
                var temp=um.Find(user,password).Id;
                return true;
            }
            catch (Exception)
            {
                return false;
            }            
            //return Membership.ValidateUser(user, password);
        }

        public bool RegisterUser(string user, string password)
        {
            try
            {
                var um = new UserManager<ApplicationUser>(new UserStore<ApplicationUser>(new ApplicationDbContext()));
                var usern = new ApplicationUser()
                {
                    UserName = user,
                };
                um.Create(usern, password);
                //um.SetEmail(usern.Id,email);
                
                //Membership.CreateUser(user, password, email);
                //WebSecurity.CreateUserAndAccount(user, password);
                return true;

            }
            catch (Exception)
            {
                return false;
            }
        }

        public string DownloadMapa(string fileName)
        {
            StringBuilder sb = new StringBuilder();


            //string filePath = "C:\\inetpub\\wwwroot\\Walk\\" + fileName;
            //Localizar o ficheiro do mapa
            string filePath = HttpContext.Current.Server.MapPath(fileName);

            using (StreamReader sr = new StreamReader(filePath))
            {
                String line;
                while ((line = sr.ReadLine()) != null)
                {
                    sb.AppendLine(line);
                }
            }
            string allLines = sb.ToString();
            return allLines;
        }

        public int GetNumberOfUsers()
        {
            int total = 0;
            using (ApplicationDbContext db = new ApplicationDbContext())
            {

                total = db.Users.Count();

            }
            return total;
        }

        public bool InsertTrack(string user, string map,string track)
        {
            using (ApplicationDbContext db = new ApplicationDbContext())
            {

                var userValidation = db.Users.Where(u => u.UserName == user);
                if (userValidation.Count() > 0)
                {
                    //var username = db.Leaderboard.Where(s => s.User == user).SingleOrDefault();
                    //if (username != null)
                    //{
                    //    username.Score = score;
                    //    db.SaveChanges();
                    //}
                    //else
                    //{
                    var us = new TrackModel();
                    us.Date = DateTime.Now;
                    us.User = user;
                    us.Map = map;
                    us.Track = track;
                    db.Track.Add(us);

                    db.SaveChanges();

                    return true;
                }
                return false;
            }
        }

        public string InfoMapa()
        {
            string retorno = "";
            using (ApplicationDbContext db = new ApplicationDbContext())
            {
                List<MapaModel> mapas = db.Mapa.ToList();
                int contMapas = mapas.Count;
                int contQuartos = 0;
                foreach (MapaModel mapa in mapas)
                {
                    if (mapa.Tipo != 0)
                    {
                        retorno += mapa.Nome + "|" + mapa.NomeFicheiro + "|";
                        contQuartos = mapa.Quartos.Count;
                        foreach (QuartoModel quarto in mapa.Quartos)
                        {
                            retorno += quarto.NomeFicheiro;
                            contQuartos--;
                            if (contQuartos != 0)
                            {
                                retorno += "|";
                            }
                        }
                        contMapas--;
                        if (contMapas != 0)
                        {
                            retorno += "!";
                        }

                    }
                }
            }
            return retorno;
        }

        public string DownloadMusica(string nome)
        {
            string retorno = "";
            using (ApplicationDbContext db = new ApplicationDbContext())
            {
                List<MusicModel> musicas = db.Music.ToList();
                foreach(MusicModel musica in musicas)
                {
                    if(musica.Nome.Equals(nome))
                    {
                        retorno = "wvm038.dei.isep.ipp.pt/website/"+musica.URL;
                        break;
                    }
                }
            }
            return retorno;
        }
        public string DownloadTextura(string nome)
        {
            string retorno = "";
            using (ApplicationDbContext db = new ApplicationDbContext())
            {
                List<TextureModel> texturas = db.Texture.ToList();
                foreach (TextureModel textura in texturas)
                {
                    if (textura.Nome.Equals(nome))
                    {
                        retorno = "wvm038.dei.isep.ipp.pt/website/" + textura.URL;
                        break;
                    }
                }
            }
            return retorno;
        }

        public string DownloadModeloInimigo(string nome)
        {
            string retorno = "";
            using (ApplicationDbContext db = new ApplicationDbContext())
            {
                List<ModeloModel> modelos = db.Modelo.ToList();
                foreach (ModeloModel modelo in modelos)
                {
                    if (modelo.Nome.Equals(nome))
                    {
                        retorno = modelo.URL;
                        break;
                    }
                }
            }
            return retorno;
        }
    }
}

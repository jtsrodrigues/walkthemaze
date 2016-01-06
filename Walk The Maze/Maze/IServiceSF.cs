using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.ServiceModel;
using System.Text;

namespace Maze
{
    // NOTE: You can use the "Rename" command on the "Refactor" menu to change the interface name "IServiceSF" in both code and config file together.
    [ServiceContract]
    public interface IServiceSF
    {
        [OperationContract]
        bool InsertUploadData(string user, int score, string mapa);
        [OperationContract]
        bool CheckLogin(string user, string password);
        [OperationContract]
        bool RegisterUser(string user, string password);
        [OperationContract]
        string DownloadMapa(string fileName);
        [OperationContract]
        int GetNumberOfUsers();
        [OperationContract]
        bool InsertTrack(string user, string map,string track);
        [OperationContract]
        string InfoMapa();
        [OperationContract]
        string DownloadMusica(string nome);
        [OperationContract]
        string DownloadTextura(string nome);
        [OperationContract]
        string DownloadModeloInimigo(string nome);
    }
}

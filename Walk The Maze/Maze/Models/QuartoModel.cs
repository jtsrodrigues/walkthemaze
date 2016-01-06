using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

namespace Maze.Models
{
    public class QuartoModel
    {
        public int ID { get; set; }
        public int MapaID { get; set; }
        public string Nome { get; set; }
        public string NomeFicheiro { get; set; }
    }
}
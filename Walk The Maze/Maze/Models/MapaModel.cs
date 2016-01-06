using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.ComponentModel;
using System.ComponentModel.DataAnnotations;

namespace Maze.Models
{
    public class MapaModel
    {
        public int ID { get; set; }
        public string Nome { get; set; }
        public string NomeFicheiro { get; set; }
        [Range(0, 1)]
        public int Tipo { get; set; }
        public string UserName { get; set; }
        public virtual ICollection<QuartoModel> Quartos { get; set; }
    }
}
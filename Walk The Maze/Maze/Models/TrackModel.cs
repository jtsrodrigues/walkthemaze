using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

namespace Maze.Models
{
    public class TrackModel
    {
        public int ID { get; set; }
        public string User { get; set; }
        public string Track { get; set; }
        public string Map { get; set; }
        public DateTime Date { get; set; }
    }
}
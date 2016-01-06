using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

namespace Maze.Models
{
    public class LeaderboardModel
    {
        public int ID { get; set; }
        public string User { get; set; }
        public int Score { get; set; }
        public DateTime ScoreDate { get; set; }
        public int MundoID { get; set; }
    }
}
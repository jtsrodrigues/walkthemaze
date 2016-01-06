using System;
using System.Collections.Generic;
using System.Data;
using System.Data.Entity;
using System.Linq;
using System.Net;
using System.Web;
using System.Web.Mvc;
using Maze.Models;

namespace Maze.Controllers
{
    public class LeaderboardController : Controller
    {
        private ApplicationDbContext db = new ApplicationDbContext();

        // GET: Leaderboard
        public ActionResult Index()
        {
            //Ordenar tabela
            var database = db.Leaderboard.OrderByDescending(s => s.Score);
            //Modificar valor na base de dados
            //var user = db.Leaderboard.Where(l => l.User == "Laurigirl").SingleOrDefault();
            //user.Score = 2100;
            //db.SaveChanges();

            return View(database.ToList());
        }

        // GET: Leaderboard/Details/5
        [Authorize(Roles = "canEdit")]
        public ActionResult Details(int? id)
        {
            if (id == null)
            {
                return new HttpStatusCodeResult(HttpStatusCode.BadRequest);
            }
            LeaderboardModel leaderboardModel = db.Leaderboard.Find(id);
            if (leaderboardModel == null)
            {
                return HttpNotFound();
            }
            return View(leaderboardModel);
        }

        // GET: Leaderboard/Create
        [Authorize(Roles = "canEdit")]
        public ActionResult Create()
        {
            return View();
        }

        // POST: Leaderboard/Create
        // To protect from overposting attacks, please enable the specific properties you want to bind to, for 
        // more details see http://go.microsoft.com/fwlink/?LinkId=317598.
        [HttpPost]
        [ValidateAntiForgeryToken]
        public ActionResult Create([Bind(Include = "ID,User,Score,ScoreDate")] LeaderboardModel leaderboardModel)
        {
            if (ModelState.IsValid)
            {
                db.Leaderboard.Add(leaderboardModel);
                db.SaveChanges();
                return RedirectToAction("Index");
            }

            return View(leaderboardModel);
        }

        // GET: Leaderboard/Edit/5
        [Authorize(Roles = "canEdit")]
        public ActionResult Edit(int? id)
        {
            if (id == null)
            {
                return new HttpStatusCodeResult(HttpStatusCode.BadRequest);
            }
            LeaderboardModel leaderboardModel = db.Leaderboard.Find(id);
            if (leaderboardModel == null)
            {
                return HttpNotFound();
            }
            return View(leaderboardModel);
        }

        // POST: Leaderboard/Edit/5
        // To protect from overposting attacks, please enable the specific properties you want to bind to, for 
        // more details see http://go.microsoft.com/fwlink/?LinkId=317598.
        [HttpPost]
        [ValidateAntiForgeryToken]
        [Authorize(Roles = "canEdit")]
        public ActionResult Edit([Bind(Include = "ID,User,Score,ScoreDate")] LeaderboardModel leaderboardModel)
        {
            if (ModelState.IsValid)
            {
                db.Entry(leaderboardModel).State = EntityState.Modified;
                db.SaveChanges();
                return RedirectToAction("Index");
            }
            return View(leaderboardModel);
        }

        // GET: Leaderboard/Delete/5
        [Authorize(Roles = "canEdit")]
        public ActionResult Delete(int? id)
        {
            if (id == null)
            {
                return new HttpStatusCodeResult(HttpStatusCode.BadRequest);
            }
            LeaderboardModel leaderboardModel = db.Leaderboard.Find(id);
            if (leaderboardModel == null)
            {
                return HttpNotFound();
            }
            return View(leaderboardModel);
        }

        // POST: Leaderboard/Delete/5
        [HttpPost, ActionName("Delete")]
        [ValidateAntiForgeryToken]
        public ActionResult DeleteConfirmed(int id)
        {
            LeaderboardModel leaderboardModel = db.Leaderboard.Find(id);
            db.Leaderboard.Remove(leaderboardModel);
            db.SaveChanges();
            return RedirectToAction("Index");
        }

        protected override void Dispose(bool disposing)
        {
            if (disposing)
            {
                db.Dispose();
            }
            base.Dispose(disposing);
        }
    }
}

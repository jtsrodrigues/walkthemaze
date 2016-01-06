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
    public class MapasController : Controller
    {
        private ApplicationDbContext db = new ApplicationDbContext();

        // GET: Mapas
        public ActionResult Index()
        {
            List<MapaModel> listaMapas = new List<MapaModel>();
            if (User.Identity.Name == "admin")
            {
                listaMapas = db.Mapa.OrderBy(m =>m.Nome).ToList();
            }
            else if(User.Identity.IsAuthenticated)
            {
                string user = User.Identity.Name;
                List<MapaModel> listaPub = db.Mapa.Where(m => m.Tipo == 1).OrderBy(m => m.Nome).ToList();
                List<MapaModel> listaPriv = db.Mapa.Where(m => m.Tipo == 0).Where(m => m.UserName == user).OrderBy(m => m.Nome).ToList();
                foreach(MapaModel m in listaPub)
                {
                    listaMapas.Add(m);
                }
                foreach (MapaModel m in listaPriv)
                {
                    listaMapas.Add(m);
                }
            }
            else
            {
                listaMapas = db.Mapa.Where(m => m.Tipo == 1).ToList();
            }
            return View(listaMapas);
        }

        // GET: Mapas/Details/5
        [Authorize(Roles = "canEdit")]
        public ActionResult Details(int? id)
        {
            if (id == null)
            {
                return new HttpStatusCodeResult(HttpStatusCode.BadRequest);
            }
            MapaModel mapaModel = db.Mapa.Find(id);
            if (mapaModel == null)
            {
                return HttpNotFound();
            }
            return View(mapaModel);
        }

        // GET: Mapas/Create
        public ActionResult Create()
        {
            return View();
        }

        // POST: Mapas/Create
        // To protect from overposting attacks, please enable the specific properties you want to bind to, for 
        // more details see http://go.microsoft.com/fwlink/?LinkId=317598.
        [HttpPost]
        [ValidateAntiForgeryToken]
        public ActionResult Create([Bind(Include = "ID,Nome,NomeFicheiro,Tipo")] MapaModel mapaModel)
        {
            if (ModelState.IsValid)
            {
                db.Mapa.Add(mapaModel);
                db.SaveChanges();
                return RedirectToAction("Index");
            }

            return View(mapaModel);
        }

        // GET: Mapas/Edit/5
        [Authorize(Roles = "canEdit")]
        public ActionResult Edit(int? id)
        {
            if (id == null)
            {
                return new HttpStatusCodeResult(HttpStatusCode.BadRequest);
            }
            MapaModel mapaModel = db.Mapa.Find(id);
            if (mapaModel == null)
            {
                return HttpNotFound();
            }
            return View(mapaModel);
        }

        // POST: Mapas/Edit/5
        // To protect from overposting attacks, please enable the specific properties you want to bind to, for 
        // more details see http://go.microsoft.com/fwlink/?LinkId=317598.
        [HttpPost]
        [ValidateAntiForgeryToken]
        [Authorize(Roles = "canEdit")]
        public ActionResult Edit([Bind(Include = "ID,Nome,NomeFicheiro,Tipo")] MapaModel mapaModel)
        {
            if (ModelState.IsValid)
            {
                db.Entry(mapaModel).State = EntityState.Modified;
                db.SaveChanges();
                return RedirectToAction("Index");
            }
            return View(mapaModel);
        }

        // GET: Mapas/Delete/5
        [Authorize(Roles = "canEdit")]
        public ActionResult Delete(int? id)
        {
            if (id == null)
            {
                return new HttpStatusCodeResult(HttpStatusCode.BadRequest);
            }
            MapaModel mapaModel = db.Mapa.Find(id);
            if (mapaModel == null)
            {
                return HttpNotFound();
            }
            return View(mapaModel);
        }

        // POST: Mapas/Delete/5
        [HttpPost, ActionName("Delete")]
        [ValidateAntiForgeryToken]
        [Authorize(Roles = "canEdit")]
        public ActionResult DeleteConfirmed(int id)
        {
            MapaModel mapaModel = db.Mapa.Find(id);
            db.Mapa.Remove(mapaModel);
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

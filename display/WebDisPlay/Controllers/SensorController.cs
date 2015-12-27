using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.Mvc;
using System.Runtime.Serialization;
using System.Web.Script.Serialization;
using System.Collections;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Threading.Tasks;
using System.Data.SqlClient;

namespace WebDisPlay.Controllers
{
    public class SensorController : Controller
    {
        public ActionResult sensor()
        {
            return View();
        }

        public string queryDatabase(string tbim, string alias)
        {
            /*  long tick = DateTime.Now.Ticks;
              Random ro = new Random((int)(tick & 0xffffffffL) | (int)(tick >> 32));
              int iResult;
              int iUp = 100;
              int iDown = 80;
              iResult = ro.Next(iDown, iUp);
              iResult.ToString();
              SensorInfo f1 = new SensorInfo();
              f1.tbim = int.Parse(tbim);
              f1.alias = int.Parse(alias);
              f1.value = double.Parse(iResult.ToString());
              f1.description = "for test";
              f1.peroid = int.Parse("100");
              List<SensorInfo> list = new List<SensorInfo>();
              list.Add(f1);
              */
            string coonString = @"SERVER=tianmn-pc\sqlexpress;UID=tianmn2109;password=10000000;";
            SqlConnection connection = new SqlConnection(coonString);
            connection.Open();
            string queryStr = @"select * from dbo.testData where tbim='" + tbim + "' and alias = '" + alias + "'";
            SqlCommand seleceCMD = new SqlCommand(queryStr, connection);
            SqlDataAdapter custDA = new SqlDataAdapter();
            custDA.SelectCommand = seleceCMD;
            DataSet custDS = new DataSet();
            custDA.Fill(custDS);
            List<SensorInfo> list = new List<SensorInfo>();
            foreach (DataRow mDr in custDS.Tables[0].Rows)
            {
                SensorInfo f1 = new SensorInfo();
                f1.tbim = int.Parse(tbim);
                f1.alias = int.Parse(alias);
                f1.value = float.Parse(mDr["value"].ToString());
                f1.description = mDr["description"].ToString();
                f1.peroid = int.Parse(mDr["peroid"].ToString());
                list.Add(f1);       
            }
            connection.Close();
            string json = new JavaScriptSerializer().Serialize(list);
            return json;

        }
        public ActionResult querySensorValue(object sender, EventArgs e)
        {
            string tbim = Request["tbim"].ToString();
            string alias = Request["alias"].ToString();
            var res = queryDatabase(tbim, alias);
            return Json(res, JsonRequestBehavior.AllowGet);
        }

        public string querySensor(string tbim, string type)
        {

            string[] senType = new string[11];
            senType[0] = "speedSensor";
            senType[1] = "heightSensor";
            senType[2] = "temperatureSensor";
            senType[3] = "pressureSensor";
            senType[4] = "FuelQuantitySensor";
            senType[5] = "solidGague";
            senType[6] = "accelerateSensor";
            senType[7] = "VUSensor";
            senType[8] = "switch";
            senType[9] = "battery";
            senType[10] = "indicator";

            /*   long tick = DateTime.Now.Ticks;
               Random ro = new Random((int)(tick & 0xffffffffL) | (int)(tick >> 32));
               List<SensorDescription> list = new List<SensorDescription>();
               for (int i = 1; i <= 20; i++) {
                   SensorDescription f = new SensorDescription();
                   f.tbim = i.ToString();
                   f.alias = i.ToString();
                   int iRan = ro.Next(0, 11);
                   f.sensorType = senType[iRan];
                   list.Add(f);
               }
               */
            string coonString = @"SERVER=tianmn-pc\sqlexpress;UID=tianmn2109;password=10000000;";
            SqlConnection connection = new SqlConnection(coonString);
            connection.Open();
            string queryStr = @"select * from dbo.testData";
            if (tbim != "ALL") {
                queryStr += @" and tbim='" + tbim + "'";
            }
            if (type != "ALL") {
                queryStr += @" and type ='" + type + "'";
            }
            SqlCommand seleceCMD = new SqlCommand(queryStr, connection);
            SqlDataAdapter custDA = new SqlDataAdapter();
            custDA.SelectCommand = seleceCMD;
            DataSet custDS = new DataSet();
            custDA.Fill(custDS);
            List<SensorDescription> list = new List<SensorDescription>();
            foreach (DataRow mDr in custDS.Tables[0].Rows)
            {
                SensorDescription f = new SensorDescription();
                f.tbim = mDr["tbim"].ToString();
                f.alias = mDr["alias"].ToString();
                f.sensorType = senType[int.Parse(mDr["sensorType"].ToString())];
                list.Add(f);
            }
            string json = new JavaScriptSerializer().Serialize(list);
            return json;

        }
        public ActionResult queryTBIM()
        {
            /*    List<string> list = new List<string>();
                list.Add("TBIM-1");
                list.Add("TBIM-2");
                list.Add("TBIM-3");
                list.Add("TBIM-4");
                list.Add("TBIM-5");
                list.Add("TBIM-6");
            */
            string coonString = @"SERVER=tianmn-pc\sqlexpress;UID=tianmn2109;password=10000000;";
            SqlConnection connection = new SqlConnection(coonString);
            connection.Open();
            string queryStr = @"select * from dbo.testID";
            SqlCommand seleceCMD = new SqlCommand(queryStr, connection);
            SqlDataAdapter custDA = new SqlDataAdapter();
            custDA.SelectCommand = seleceCMD;
            DataSet custDS = new DataSet();
            custDA.Fill(custDS);
           
            List<string> list = new List<string>();
            foreach (DataRow mDr in custDS.Tables[0].Rows)
            {
                string f = "TBIM-";
                f += mDr["id"].ToString();
                list.Add(f);
            }
            string json = new JavaScriptSerializer().Serialize(list);
            return Json(json, JsonRequestBehavior.AllowGet);
        }
        public ActionResult querySensorType(object sender, EventArgs e)
        {
            string tbim;
            string type;
            if (Request["tbim"] == null)
            {
                tbim = "ALL";
            }
            else {
                tbim = Request["tbim"].ToString();
            }
            if (Request["type"] == null)
            {
                type = "ALL";
            }
            else {
                type = Request["type"].ToString();
            }
            
            var res = querySensor(tbim, type);
            return Json(res, JsonRequestBehavior.AllowGet);
        }

    }

    public class SensorInfo {
        public int tbim;
        public int alias;
        public double value;
        public string description;
        public int peroid;
    }

    public class SensorDescription {
        public string tbim;
        public string alias;
        public string sensorType;
    }

    public class tbim {
        public string id;
    }
}
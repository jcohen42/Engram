using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.IO;
using System.Diagnostics;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using ScintillaNET;

namespace EngramIDEForm
{
    public partial class Form1 : Form
    {
        [DllImport("Gdi32.dll", EntryPoint = "CreateRoundRectRgn")]
        private static extern IntPtr CreateRoundRectRgn
        (
            int nLeftRect,     // x-coordinate of upper-left corner
            int nTopRect,      // y-coordinate of upper-left corner
            int nRightRect,    // x-coordinate of lower-right corner
            int nBottomRect,   // y-coordinate of lower-right corner
            int nWidthEllipse, // height of ellipse
            int nHeightEllipse // width of ellipse
        );

        Process compiler;
        StreamWriter myStreamWriter;
        string inputBuffer = "";

        int OUTPUT = 0;
        int INPUT = 1;
        int MAXIO = 1024;

        ScintillaNET.Scintilla editor;

        private void backgroundWorker1_DoWork(object sender, DoWorkEventArgs e)
        {
            BackgroundWorker worker = sender as BackgroundWorker;

            ProcessStartInfo info = new ProcessStartInfo();
            info.FileName = "compiler.exe";
            info.Arguments = "engram.txt -showInputFlag";
            info.UseShellExecute = false;
            info.RedirectStandardOutput = true;
            info.RedirectStandardInput = true;
            info.CreateNoWindow = true;

            compiler = Process.Start(info);
            myStreamWriter = compiler.StandardInput;

            int ioCount = 0;
            while (!compiler.HasExited)
            {
                if (worker.CancellationPending)
                {
                    e.Cancel = true;
                    break;
                }

                if(ioCount >= MAXIO)
                {
                    worker.ReportProgress(OUTPUT, "The program exited prematurely because an infinite loop was encountered.");
                    myStreamWriter.Close();
                    compiler.Close();
                    return;
                }
                
                string output = compiler.StandardOutput.ReadLine();
                if (output == "!@#INPUT#@!")
                {
                    while (inputBuffer == "") { if (worker.CancellationPending) return; }
                    worker.ReportProgress(INPUT);
                }
                else
                {
                    worker.ReportProgress(OUTPUT, output);
                }

                ioCount++;
            }
            worker.ReportProgress(OUTPUT, compiler.StandardOutput.ReadToEnd());

            myStreamWriter.Close();
            compiler.WaitForExit();
            compiler.Close();
        }

        private void backgroundWorker1_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            if(e.ProgressPercentage == OUTPUT)
            {
                outputTextBox.Text += (e.UserState as String) + "\n";
            } 
            else
            {
                myStreamWriter.WriteLine(inputBuffer);
                inputBuffer = "";
            }
        }

        private void backgroundWorker1_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            if (e.Cancelled == true)
            {
                debugLabel.Text = "Canceled!";
            }
            else if (e.Error != null)
            {
                debugLabel.Text = "Error: " + e.Error.Message;
            }
            else
            {
                debugLabel.Text = "Done!";
            }
            
            stopButton.Enabled = false;
            runButton.Enabled = true;
            runButton.Text = "Run";
        }

        public Form1()
        {
            InitializeComponent();

            //Create rounded corners for each text field, button, and the window itself
            Region = Region.FromHrgn(CreateRoundRectRgn(0, 0, Width, Height, 20, 20));
            outputTextBox.Region = Region.FromHrgn(CreateRoundRectRgn(0, 0, outputTextBox.Width, outputTextBox.Height, 20, 20));
            runButton.Region = Region.FromHrgn(CreateRoundRectRgn(0, 0, runButton.Width, runButton.Height, 10, 10));
            stopButton.Region = Region.FromHrgn(CreateRoundRectRgn(0, 0, stopButton.Width, stopButton.Height, 10, 10));
            saveButton.Region = Region.FromHrgn(CreateRoundRectRgn(0, 0, saveButton.Width, saveButton.Height, 10, 10));
            loadButton.Region = Region.FromHrgn(CreateRoundRectRgn(0, 0, loadButton.Width, loadButton.Height, 10, 10));
            enterButton.Region = Region.FromHrgn(CreateRoundRectRgn(0, 0, enterButton.Width, enterButton.Height, 10, 10));
            //buttonExit.Region = Region.FromHrgn(CreateRoundRectRgn(0, 0, buttonExit.Width, buttonExit.Height, 20, 20));

            SendMessage(inputTextBox.Handle, 0x1501, 1, "Type a number...");
            SendMessage(editorTextBox.Handle, 0x1501, 1, "Write your program here!");

            this.ActiveControl = editorTextBox;
            editorTextBox.Focus();

            backgroundWorker1.WorkerReportsProgress = true;
            backgroundWorker1.WorkerSupportsCancellation = true;


                editor = new Scintilla();
                editor.Width = 300;
                editor.Height = 300;
                editor.Text = "TEST";
                editor.Visible = true;
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            // CREATE CONTROL
            //TextArea = new ScintillaNET.Scintilla();
        }

        //Drag form
        //https://rjcodeadvance.com/crear-formulario-de-bordes-redondeados-suaves-c-winforms/
        [DllImport("user32.DLL", EntryPoint = "ReleaseCapture")]
        private extern static void ReleaseCapture();
        [DllImport("user32.DLL", EntryPoint = "SendMessage")]
        private extern static void SendMessage(System.IntPtr hWnd, int wMsg, int wParam, int lParam);
        private void panelTitleBar_MouseDown(object sender, MouseEventArgs e)
        {
            ReleaseCapture();
            SendMessage(this.Handle, 0x112, 0xf012, 0);
        }

        //Hint text for
        // Within your class or scoped in a more appropriate location:
        [DllImport("user32.dll")]
        private static extern IntPtr SendMessage(IntPtr hWnd, int Msg, int wParam, [MarshalAs(UnmanagedType.LPWStr)] string lParam);

        private void runButton_Click(object sender, EventArgs e)
        {
            //Prepare for running the program
            outputTextBox.Text = "";
            runButton.Text = "Running...";
            runButton.Enabled = false;
            stopButton.Enabled = true;

            //Save the program to a file
            FileInfo myFile = new FileInfo("engram.txt");
            myFile.Attributes &= ~FileAttributes.Hidden;
            File.WriteAllLines("engram.txt", editorTextBox.Text.Split('\n'));
            myFile.Attributes |= FileAttributes.Hidden;

            //Empty the input buffer
            inputBuffer = "";

            //Use the background worker to run the program
            backgroundWorker1.RunWorkerAsync();
        }

        private void inputTextBox_KeyDown(object sender, KeyEventArgs e)
        {
            //If the enter key is pressed, act as if the enter button was pressed
            if(e.KeyCode == Keys.Enter)
            {
                enterButton_Click(sender, e);

                e.SuppressKeyPress = true; //Stops the 'ding' noise
            }
        }

        private void enterButton_Click(object sender, EventArgs e)
        {
            //Store the input text box value to the inputBuffer
            inputBuffer = inputTextBox.Text;
            inputTextBox.Text = "";
            debugLabel.Text = "Enter";
        }

        private void stopButton_Click(object sender, EventArgs e)
        {
            //Stop the running program
            if(backgroundWorker1.IsBusy)
                debugLabel.Text = "Cancelling";
                backgroundWorker1.CancelAsync();
        }

        private void loadButton_Click(object sender, EventArgs e)
        {
            //Prompt the user to open a file
            openFileDialog.Filter = "Text|*.txt";
            openFileDialog.ShowDialog();

            //Open the file
            string fileName = openFileDialog.FileName;

            //Pass the lines to the editor text box
            string[] lines = File.ReadAllLines(fileName);
            editorTextBox.Text = "";
            for(int i = 0; i < lines.Length; i++)
            {
                editorTextBox.Text += lines[i] + "\n";
            }
        }

        private void saveButton_Click(object sender, EventArgs e)
        {
            //Save the editor text box to the computer
            saveFileDialog.Filter = "Text|*.txt";
            saveFileDialog.Title = "Save your program";
            saveFileDialog.FileName = "engram";
            saveFileDialog.ShowDialog();

            File.WriteAllLines(saveFileDialog.FileName, editorTextBox.Text.Split('\n'));
        }

        private void buttonExit_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }
    }
}

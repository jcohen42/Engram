/*
 * Jake Cohen
 * Arizona State University
 * Engram IDE
 */

using System;
using System.ComponentModel;
using System.Drawing;
using System.IO;
using System.Diagnostics;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using ScintillaNET;

namespace EngramIDE
{
    public partial class Form1 : Form
    {
        Process compiler;
        StreamWriter myStreamWriter;
        string inputBuffer = "";

        int OUTPUT = 0;
        int INPUT = 1;
        int MAXIO = 2048;

        //Start the Engram compiler as a Process. Display output and gather output
        private void backgroundWorker1_DoWork(object sender, DoWorkEventArgs e)
        {
            BackgroundWorker worker = sender as BackgroundWorker;

            ProcessStartInfo info = new ProcessStartInfo();
            info.FileName = "compiler.exe";
            string path = Path.GetTempPath();
            info.Arguments = path + "engram.txt -showInputFlag";
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

                if (ioCount >= MAXIO)
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

        //Append output to outputTextBox and gather user input if necessary
        private void backgroundWorker1_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            if (e.ProgressPercentage == OUTPUT)
            {
                outputTextBox.Text += (e.UserState as String) + "\n";
            }
            else
            {
                myStreamWriter.WriteLine(inputBuffer);
                inputBuffer = "";
            }
        }

        //Once the program has terminated, we can re-enable the Run button and disable the Stop button
        private void backgroundWorker1_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            stopButton.Enabled = false;
            runButton.Enabled = true;
            runButton.Text = "Run";
        }

        //This method was adapted from: https://github.com/jacobslusser/ScintillaNET/wiki/Automatic-Syntax-Highlighting
        //Set the color and styling for the tokens of the Engram language
        public void setEditorHighlighting()
        {
            //Configure the default style properties
            editorTextBox.StyleResetDefault();
            editorTextBox.Styles[Style.Default].BackColor = Color.FromArgb(1, 184, 190, 221);
            editorTextBox.Styles[Style.Default].Font = "Segoe UI";
            editorTextBox.Styles[Style.Default].Size = 14;
            editorTextBox.StyleClearAll();
            
            //Configure the C++ lexer styles
            //I use C++ because Engram and C++ and somewhat lixically ismilar
            editorTextBox.Styles[Style.Cpp.Default].ForeColor = Color.Silver;
            editorTextBox.Styles[Style.Cpp.Number].ForeColor = Color.FromArgb(0, 128, 0); // Green - numbers
            editorTextBox.Styles[Style.Cpp.Word].ForeColor = Color.Blue; // Primary keywords and identifiers
            editorTextBox.Styles[Style.Cpp.Word2].ForeColor = Color.Purple; // Secondary keywords and identifiers
            editorTextBox.Styles[Style.Cpp.String].ForeColor = Color.FromArgb(163, 21, 21); // Red
            editorTextBox.Styles[Style.Cpp.StringEol].BackColor = Color.Pink;
            editorTextBox.Lexer = Lexer.Cpp;

            editorTextBox.SetSelectionBackColor(true, Color.FromArgb(255, 255, 50));

            //Set the keywords
            //-Primary keywords and identifiers
            editorTextBox.SetKeywords(0, "Set Call While while End Begin If if Else with argument Return program");
            //-Secondary keywords and identifiers
            editorTextBox.SetKeywords(1, "equal to not is less greater than plus minus times divided by modulo Output Input set return");
        }

        //Create a rounded control with height and width
        [DllImport("Gdi32.dll", EntryPoint = "CreateRoundRectRgn")]
        private static extern IntPtr CreateRoundRectRgn (
            int nLeftRect,     // x-coordinate of upper-left corner
            int nTopRect,      // y-coordinate of upper-left corner
            int nRightRect,    // x-coordinate of lower-right corner
            int nBottomRect,   // y-coordinate of lower-right corner
            int nWidthEllipse, // height of ellipse
            int nHeightEllipse // width of ellipse
        );

        //Prepare the form for displaying
        public Form1()
        {
            InitializeComponent();

            //Set line number margins
            editorTextBox.Margins[0].Width = 24;
            editorTextBox.Margins[1].Width = 0;

            //Prepare for syntax highlighting
            setEditorHighlighting();

            //Create rounded corners for each text field, button, and the window itself
            Region = Region.FromHrgn(CreateRoundRectRgn(0, 0, Width, Height, 20, 20));
            outputTextBox.Region = Region.FromHrgn(CreateRoundRectRgn(0, 0, outputTextBox.Width, outputTextBox.Height, 20, 20));
            panelOutputBG.Region = Region.FromHrgn(CreateRoundRectRgn(0, 0, panelOutputBG.Width, panelOutputBG.Height, 20, 20));
            editorTextBox.Region = Region.FromHrgn(CreateRoundRectRgn(0, 0, editorTextBox.Width, editorTextBox.Height, 20, 20));
            runButton.Region = Region.FromHrgn(CreateRoundRectRgn(0, 0, runButton.Width, runButton.Height, 10, 10));
            stopButton.Region = Region.FromHrgn(CreateRoundRectRgn(0, 0, stopButton.Width, stopButton.Height, 10, 10));
            saveButton.Region = Region.FromHrgn(CreateRoundRectRgn(0, 0, saveButton.Width, saveButton.Height, 10, 10));
            loadButton.Region = Region.FromHrgn(CreateRoundRectRgn(0, 0, loadButton.Width, loadButton.Height, 10, 10));
            enterButton.Region = Region.FromHrgn(CreateRoundRectRgn(0, 0, enterButton.Width, enterButton.Height, 10, 10));

            //Add hint text to the input text box
            SendMessage(inputTextBox.Handle, 0x1501, 1, "Type a number...");

            //Set the focus to the editor
            this.ActiveControl = editorTextBox;
            editorTextBox.Focus();
        }

        //Drag form - adapter from: https://rjcodeadvance.com/crear-formulario-de-bordes-redondeados-suaves-c-winforms/
        [DllImport("user32.DLL", EntryPoint = "ReleaseCapture")]
        private extern static void ReleaseCapture();
        [DllImport("user32.DLL", EntryPoint = "SendMessage")]
        private extern static void SendMessage(System.IntPtr hWnd, int wMsg, int wParam, int lParam);
        private void labelTitle_MouseDown(object sender, MouseEventArgs e)
        {
            ReleaseCapture();
            SendMessage(this.Handle, 0x112, 0xf012, 0);
        }

        //This extern method allows us to display hint text on the inputTextBox
        [DllImport("user32.dll")]
        private static extern IntPtr SendMessage(IntPtr hWnd, int Msg, int wParam, [MarshalAs(UnmanagedType.LPWStr)] string lParam);

        //Run the Engram compiler as a Background Worker
        private void runButton_Click(object sender, EventArgs e)
        {
            //Prepare for running the program
            outputTextBox.Text = "";
            runButton.Text = "Running...";
            runButton.Enabled = false;
            stopButton.Enabled = true;

            //Save the program to a file
            string path = Path.GetTempPath();
            File.WriteAllText(path + "engram.txt", editorTextBox.Text);

            //Empty the input buffer
            inputBuffer = "";

            //Use the background worker to run the program
            backgroundWorker1.RunWorkerAsync();
        }

        //Checks if the user types Enter in the inputTextBox, and calls the Enter Button method if so
        private void inputTextBox_KeyDown(object sender, KeyEventArgs e)
        {
            //If the enter key is pressed, act as if the enter button was pressed
            if (e.KeyCode == Keys.Enter)
            {
                enterButton_Click(sender, e);

                e.SuppressKeyPress = true; //Stops the 'ding' noise
            }
        }

        //Store the input text box value to the inputBuffer
        private void enterButton_Click(object sender, EventArgs e)
        {
            inputBuffer = inputTextBox.Text;
            inputTextBox.Text = "";
        }

        //Stop the running program
        private void stopButton_Click(object sender, EventArgs e)
        {
            backgroundWorker1.CancelAsync();
        }

        //Loads a .txt file using File Explorer and displays it in the editor
        private void loadButton_Click(object sender, EventArgs e)
        {
            //Prompt the user to open a file
            openFileDialog.Filter = "Text|*.txt";
            openFileDialog.ShowDialog();

            //Open the file
            string fileName = openFileDialog.FileName;

            try
            {
                //Pass the lines to the editor text box
                string[] lines = File.ReadAllLines(fileName);
                editorTextBox.Text = "";
                for (int i = 0; i < lines.Length; i++)
                {
                    editorTextBox.Text += lines[i] + "\n";
                }
            } catch (FileNotFoundException err) { }
        }

        //Saves the text written in the editor to engram.txt by default
        private void saveButton_Click(object sender, EventArgs e)
        {
            //Save the editor text box to the computer
            saveFileDialog.Filter = "Text|*.txt";
            saveFileDialog.Title = "Save your program";
            saveFileDialog.FileName = "engram";
            saveFileDialog.ShowDialog();

            File.WriteAllLines(saveFileDialog.FileName, editorTextBox.Text.Split('\n'));
        }

        //Exits the IDE application
        private void buttonExit_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }
    }
}

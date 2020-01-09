
using System;
using UnityEngine;
using System.Collections.Generic;
using System.Text.RegularExpressions;

namespace RedDev.Utils
{
	/// Класс отвечает за разбор данных из csv-файлов.
	/// Кодировка должна быть UTF-8.
	public class CsvData
	{
		/// Заголовочный блок.
		public readonly List<string> Headers = new List<string>();

		/// Основная информация файла.
		public readonly List<List<string>> Lines = new List<List<string>>();

		/// Разбирает текстовую информацию.
		/// <param name="data">Текстовая информация из csv-файлов.</param>
		/// <param name="headers">Содержит ли информация заголовочный раздел.</param>
		/// <returns>Возвращает объект с разделённой информацией по строкам и столбцам.</returns>
		public static CsvData Parse(string data, bool headers = true)
		{
			//string[] lines = data.Split(new string[] { "\r\n" }, System.StringSplitOptions.None);
			string[] lines = data.Split(new char[] { '\n' });

			if (headers)
			{
				List<string> tmp = new List<string>();
				for (int i = 1; i < lines.Length; i++)
					tmp.Add(lines[i]);
				return new CsvData(lines[0], tmp);
			}
			else
				return new CsvData(lines);
		}

		/// Разбирает одну строку из csv-файла.
		/// <param name="line">Строка.</param>
		/// <returns>Возвращает список информации разбитый по колонкам.</returns>
		private static List<string> ParseCsvLine(string line)
		{
			string pattern = @"
				# Match one value in valid CSV string.
				(?!\s*$)                                      # Don't match empty last value.
				\s*                                           # Strip whitespace before value.
				(?:                                           # Group for value alternatives.
				'(?<val>[^'\\]*(?:\\[\S\s][^'\\]*)*)'         # Either $1: Single quoted string,
				| ""(?<val>[^""\\]*(?:\\[\S\s][^""\\]*)*)""   # or $2: Double quoted string,
				| (?<val>[^,'""\s\\]*(?:\s+[^,'""\s\\]+)*)    # or $3: Non-comma, non-quote stuff.
				)                                             # End group of value alternatives.
				\s*                                           # Strip whitespace after value.
				(?:,|$)                                       # Field ends on comma or EOS.";
			try
			{
				/*List<string> values = (from Match m in Regex.Matches(line, pattern, 
					RegexOptions.ExplicitCapture | RegexOptions.IgnorePatternWhitespace | RegexOptions.Multiline)
					select m.Groups[1].Value).Select(x => x.Replace("<br/>", "\r\n")).ToList();
				return values;*/
				List<string> values = new List<string>();
				MatchCollection matches = Regex.Matches(line, pattern, RegexOptions.ExplicitCapture | RegexOptions.IgnorePatternWhitespace | RegexOptions.Multiline);

				for (int i = 0; i < matches.Count; i++)
					values.Add(matches[i].Groups[1].Value.Replace("<br/>", "\r\n"));

				return values;
			}
			catch (Exception except)
			{
				Debug.Log("Ошибка разбора строк: " + except.Message);

				return new List<string>();
			}
		}

		/// Конструктор объекта.
		/// <param name="lines">Массив строк из csv-файла.</param>
		public CsvData(string[] lines)
		{
			for (int i = 0; i < lines.Length; i++)
				Lines.Add(CsvData.ParseCsvLine(lines[i]));
		}

		/// Конструктор объекта.
		/// <param name="headers">Строка заголовка csv-файла.</param>
		/// <param name="lines">Остальные строки csv-файла.</param>
		public CsvData(string headers, List<string> lines)
		{
			Headers = CsvData.ParseCsvLine(headers);

			for (int i = 0; i < lines.Count; i++)
			{
				List<string> s = CsvData.ParseCsvLine(lines[i]);
				Lines.Add(s);
			}
		}

		/// Возвращает столбец информации по заголовку.
		/// <param name="header">Заголовок столбца.</param>
		/// <returns>Возвращает строковый список стобца с указанным заголовком.</returns>
		public List<string> GetColumn(string header)
		{
			if ((!Headers.Contains(header)) || (Lines.Count == 0))
				return null;

			int index = Headers.IndexOf(header);

			List<string> result = new List<string>();
			foreach (List<string> line in Lines)
			{
				result.Add(line[index]);
			}

			return result;
		}

		/// Возвращает столбец информации по номеру столбца.
		/// <param name="index">Номер заголовка.</param>
		/// <returns>Возращается строковой список указанного столбца.</returns>
		public List<string> GetColumn(int index)
		{
			if (Lines.Count == 0)
				return null;

			if ((index < 0) || (index >= Lines[0].Count))
				return null;

			List<string> result = new List<string>();
			foreach (List<string> line in Lines)
			{
				if (index < line.Count)
					result.Add(line[index]);
				else
					result.Add("");
			}

			return result;
		}
	}
}
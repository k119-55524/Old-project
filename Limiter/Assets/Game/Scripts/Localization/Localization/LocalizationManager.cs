
using System;
using System.IO;
using UnityEngine;
using RedDev.Utils;
using RedDev.Helpers;
using System.Collections;
using System.Collections.Generic;

namespace RedDev.Localization
{
    /// Тип описывает структуру аргументов события смены текущего языка.
    public class ChangeEventType : EventArgs
    {
        public string OldLanguage { get; private set; }
        public string NewLanguage { get; private set; }

        public ChangeEventType(string oldLanguage, string newLanguage)
        {
            OldLanguage = oldLanguage;
            NewLanguage = newLanguage;
        }
    }

	/// Класс реализаует возможность использования локализации текстовых данных.
	/// По умолчанию загружается locale_en.
	public class LocalizationManager : Singleton<LocalizationManager>
    {
        private Dictionary<string, string> _localizedText;

        public Dictionary<string, LocalizedText> _localizedObjects = new Dictionary<string, LocalizedText>();

        public bool _isReady = false;

        public string CurrentLocale = "";

        public readonly string MissingText = "Localized text not found";

        public event EventHandler<ChangeEventType> EventChangeLanguage;

        void Awake()
        {
			//StartCoroutine(LoadLocalization("en_US"));
		}

        /// Метод загружает данные конкретной локализации.
        /// <param name="locale">Название локализации</param>
        public IEnumerator LoadLocalization(string locale)
        {
            string oldLanguage = CurrentLocale;
            CurrentLocale = locale;
            _isReady = false;

            if (_localizedText == null)
                _localizedText = new Dictionary<string, string>();

            // данные должны храниться в Assets\StreamingAssets
            string filePath = "";
#if UNITY_STANDALONE_WIN
//#if UNITY_EDITOR
			filePath = "file://" + Path.Combine(Application.streamingAssetsPath, "locale.json");
#elif UNITY_STANDALONE_OSX
			filePath = "file://" + Application.streamingAssetsPath + "/locale.json";
#elif UNITY_STANDALONE
			/*filePath = "file:/" +*/ Path.Combine(Application.streamingAssetsPath, "locale.json");
#elif UNITY_ANDROID
			filePath = Application.streamingAssetsPath + "/locale.json";
#elif UNITY_IOS
			filePath = "file:" + Application.streamingAssetsPath + "/Raw/locale.json";
#elif UNITY_WEBGL
			filePath = Application.streamingAssetsPath + "/locale.json";
#else
			filePath = "file:/" + Path.Combine(Application.streamingAssetsPath, "locale.json");
#endif
            //Debug.Log("Locale file: " + filePath);

            WWW wwwLocale = new WWW(filePath);

            yield return wwwLocale;

            if (String.IsNullOrEmpty(wwwLocale.error))
            {
                _localizedText.Clear();
                LocaleData Data = JsonUtility.FromJson<LocaleData>(wwwLocale.text);

                for (int i = 0; i < Data.Files.Length; i++)
                    yield return ParseSingleLocale(Data.Files[i]);
            }
            else
            {
                Debug.LogError("Error parse locale.json: " + wwwLocale.error);
            }

            wwwLocale.Dispose();

            _isReady = true;
            ReinitLocalized();
            OnEventChangeLanguage(new ChangeEventType(oldLanguage, CurrentLocale));
		}

        private IEnumerator ParseSingleLocale(string file)
        {
			// данные должны храниться в Assets\StreamingAssets

			string filePath = "";
#if UNITY_STANDALONE_WIN
//#if UNITY_EDITOR
			filePath = "file://" + Path.Combine(Application.streamingAssetsPath, file + ".csv");
#elif UNITY_STANDALONE_OSX
			filePath = "file://" + Application.streamingAssetsPath + "/" + file + ".csv";
#elif UNITY_ANDROID
			// на андройде не работал Application.streamingAssetsPath
			filePath = Application.streamingAssetsPath + "/" + file + ".csv";
#elif UNITY_IOS
			filePath = "file:" + Application.dataPath + "/Raw/" + file + ".csv";
#elif UNITY_WEBGL
			filePath = Application.streamingAssetsPath + "/" + file + ".csv";
#else
			filePath = "file:/" + Path.Combine(Application.streamingAssetsPath, file + ".csv");
#endif
			//Debug.Log("Localization file: " + filePath);

			// на сколько понял из документации в андройде данные находятся в сжатом виде, и лучшим способом будет считывать данные через WWW.
			WWW www = new WWW(filePath);

            yield return www;

            if (String.IsNullOrEmpty(www.error))
            {
                CsvData data = CsvData.Parse(www.text);
                if (data == null)
                    Debug.LogError("Ошибка!");
                else
                {
                    if (!data.Headers.Contains("Keys"))
                    {
                        Debug.LogError("Отсутвует системный заголовок Keys");
                        //continue;
                        yield return null;
                    }
                    if (!data.Headers.Contains(CurrentLocale))
                    {
                        Debug.LogError("Отсутствует необходимая локализация: " + CurrentLocale);
                        //continue;
                        yield return null;
                    }
                    else
                    {
                        // заполняем основной словарь данными выбранной локализации
                        List<string> keys = data.GetColumn(data.Headers.IndexOf("Keys"));
                        List<string> loc = data.GetColumn(data.Headers.IndexOf(CurrentLocale));

                        for (int i = 0; i < keys.Count; i++)
                        {
                            if (_localizedText.ContainsKey(keys[i]))
                            {
                                //Debug.LogWarning(
                                //    System.String.Format(
                                //        "Ключ {0} уже встречается в локализации, будет произведена замена!", keys[i]));

                                continue;
                            }
                            try
                            {
                                if (!string.IsNullOrEmpty(keys[i]) && (!System.String.IsNullOrEmpty(loc[i])))
                                    _localizedText.Add(keys[i], loc[i]);
                            }
                            catch (Exception e)
                            {
                                Debug.LogError("Localization add " + file + " in key dictionary " + keys[i] +
                                               " exception: " + e.Message);
                            }
                        }
                    }
                }
                //Debug.Log("Data loaded, dictionary contains: " + _localizedText.Count + " entries");
            }
            else
            {
                Debug.LogError("Cannot found localization file " + filePath + ": " + www.error);
            }

            www.Dispose();
        }

        /// Метод возвращает значение перевода по ключу.
        /// <param name="key">Ключ по которому ищется переведённое значение.</param>
        /// <returns>Перевод выбранного языка.</returns>
        public string GetValue(string key)
        {
            string result = MissingText;

			if ((_localizedText != null) && (_localizedText.ContainsKey(key)))
				result = _localizedText[key];
			else
			{
				if (_isReady)
					Debug.LogError("Отсутствует ключ: " + key);
				else
					return "download...";
			}

            return result;
        }

        /// Метод производит переназначение переведённого текста у всех необходимых объектов.
        public void ReinitLocalized()
        {
            foreach (string key in _localizedObjects.Keys)
                _localizedObjects[key].ReLocalization();
            //Player.Instance.CreateOther();
        }

        /// Возвращает True, если менеджер готов к использованию.
        /// <returns>True, если локализация загружена и обработана.</returns>
        public bool IsReady()
        {
            return _isReady;
        }

        public void AddLocalizedText(LocalizedText text)
        {
            string idLocalization = text.GetIdLocalization();

            if (_localizedObjects.ContainsKey(idLocalization))
            {
                Debug.LogError("Ключ объекта локализации уже присутствует в словаре локализированных объектов: " +
                               idLocalization);

                return;
            }

            _localizedObjects.Add(idLocalization, text);
        }

        public void RemoveLocalizedText(string id)
        {
            if (_localizedObjects.ContainsKey(id))
                _localizedObjects.Remove(id);
            else
                Debug.LogError(
                    "Произошла попытка удалить объект с отсутствующим ключём в словаре локализированных объектов: " + id);
        }

        protected virtual void OnEventChangeLanguage(ChangeEventType e)
        {
            var handler = EventChangeLanguage;

            if (handler != null)
				handler(this, e);
        }
    }
}

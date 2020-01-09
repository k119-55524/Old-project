
using System;
using UnityEngine;
using UnityEngine.UI;

namespace RedDev.Localization
{
    public class LocalizedText : MonoBehaviour
    {
        public string Key;
        private bool _activated = false;
        private readonly string _idLocalization = Guid.NewGuid().ToString();
        private Text _text;

        void Start()
        {
            LocalizationManager.Instance.AddLocalizedText(this);
            ReLocalization();
        }

        public void ReLocalization()
        {
            if (_text == null)
                _text = GetComponent<Text>();

            _text.text = LocalizationManager.Instance.GetValue(Key);
        }

        void Update()
        {
            if ((gameObject.activeSelf == false) && (_activated == true))
                _activated = false;
			else if ((gameObject.activeSelf == true) && (_activated == false))
            {
                ReLocalization();
                _activated = true;
            }
        }

        void OnDestroy()
        {
            try
            {
                LocalizationManager.Instance.RemoveLocalizedText(_idLocalization);
            }
            catch
            {
                // ignored
            }
        }

        public string GetIdLocalization()
        {
            return _idLocalization;
        }

        public void Relocalize(string newKey)
        {
            Key = newKey;
            ReLocalization();
        }
    }
}
<?php
/**
 * @file
 * Custom tokens for Metatag:hreflang.
 */

/**
 * Implements hook_token_info().
 */
function metatag_hreflang_token_info() {
  // This only makes sense if either the Translation or Entity Translation
  // modules are enabled.
  if (!(module_exists('translation') || module_exists('entity_translation'))) {
    return;
  }

  $info = array();

  $languages = language_list('enabled');

  // Verify there are multiple languages enabled.
  if (!empty($languages[1]) && is_array($languages[1]) && count($languages[1]) > 1) {
    if (module_exists('node')) {
      foreach ($languages[1] as $langcode => $language) {
        $info['tokens']['node']['url-' . $langcode] = array(
          'name' => t('URL (@lang translation)', array('@lang' => $language->name)),
          'description' => t('The URL for the %lang translation of the node, if available.', array('%lang' => $language->name)),
        );
      }
    }
  }

  return $info;
}

/**
 * Implements hook_tokens().
 */
function metatag_hreflang_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $replacements = array();

  $sanitize = !empty($options['sanitize']);

  // Node tokens.
  if ($type == 'node' && !empty($data['node'])) {
    // Shortcuts.
    $node = $data['node'];

    // Only generate tokens if there are multiple translations.
    if (isset($node->translations) && !empty($node->translations->data)) {
      $languages = language_list('enabled');
      if (!empty($languages[1]) && is_array($languages[1]) && count($languages[1]) > 1) {
        foreach ($tokens as $name => $original) {
          foreach ($node->translations->data as $langcode => $translation) {
            if ($name == 'url-' . $langcode) {
              $url_options = $options;
              $url_options['language'] = $languages[1][$langcode];
              $url_options['absolute'] = TRUE;
              $replacements[$original] = url('node/' . $node->nid, $url_options);
            }
          }
        }
      }
    }
  }

  return $replacements;
}
